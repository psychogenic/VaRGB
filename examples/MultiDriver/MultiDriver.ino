/*

 VaRGB library example usage.
 Copyright (C) 2013 Pat Deegan.  All rights reserved.
 
 http://www.flyingcarsandstuff.com/projects/vargb/
 
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.
 
 See file LICENSE.txt for further informations on licensing terms.
 
 *****************************  OVERVIEW  ***************************** 

 This is the only example that actually requires you have the TinyBrite
 library installed.
 
 In this example we use a chain of 3 MegaBrites (or ShiftBrites), actual 
 RGB LEDs, through the TinyBrite library (which you may need to get and
 install, from http://flyingcarsandstuff.com/projects/tinybrite/ ).
 
 We'll be using two VaRGB objects to control the lights in this pattern:
 
      A --- B --- A
 
 So one instance of VaRGB will be in charge of setting the values for 
 the two ends, and the other will set the color for the center RGB LED.
 
 The details of using the Shift/MegaBrites and the TinyBrite library are
 only touched upon briefly--see the TinyBrite home for details on that
 stuff.
 
 *****************************  HW SETUP  *****************************  
 
 The following code expects an Arduino-compatible device tied to a
 chain of three *Brites (can be ShiftBrite, MegaBrite) or anything
 that uses the A6281 PWM LED driver.
 
 Basic hardware setup (power connections/voltage regulation for the
 Arduino are not shown):
 
 
 Arduino                      *Brite device
 +===========+datapin         +============+ DO (data out)
 |           |----------------|DI (data)   |-------------->
 | Ardweeny  |                |            | EO (enable out)
 |           |          ------|EI (enable) |-------------->
 |           |          |     |            |
 |           |         GND    |            |
 |           |clockpin        |            | CO (clock out)
 |  or any   |----------------|CI (clock)  |-------------->  To second *Brite...
 |   type    |latchpin        |            | LO (latch out)
 |  *duino   |----------------|LI (latch)  |-------------->
 +===========+                +============+
 
 
 *** NOTE ***: you may want to use pull up resistors on the 
 clock and data pins (pins A and B, above).
 This just means putting a (few k) resistor between +5V and 
 each pin. 
 
 */




/* *** Includes *** */

// vaRGB includes
#include <VaRGB.h>
#include <VaRGBCurves.h>

// We'll use TinyBrite to set some MegaBrites
#include <TinyBrite.h>


// *** DEBUG ***
// leave the define below uncommented to send 
// color adjustment debug info to Serial
// 
#define DEBUG_TO_SERIAL

#define serial_baud_rate	115200


// a few useful defines
#define megabrite_num_modules	3

// the pins we're using to talk to the 'brites
#define brite_clockpin		3
#define brite_datapin		4
#define brite_latchpin		5

// some defines to clarify the code
#define megabrite_left_idx	0
#define megabrite_center_idx	1
#define megabrite_right_idx	2




/* MegaBrites
 ** To control the RGB LEDs, we'll create a TinyBrite global object.
 **
 ** You can use any method you like in the set-color callback to control
 ** your R-G-B settings, I just have a particular fondness for the MegaBrites
 ** and the TinyBrite lib ('cause, you know, I wrote it ;-) ).
 **
 **
 ** See the TinyBrite docs for info on this 
 ** (http://flyingcarsandstuff.com/projects/tinybrite/), the short version
 ** is that we call the constructor with the number of brites in the chain.
 */

TinyBrite Brite_chain(megabrite_num_modules);





/* *** The VaRGB driver object *** */

// callback declarations.
// We need a color-setting callback and a another 
// callback for when a schedule has run its course, 
// in order to create our VaRGB driver.

// these are forward declarations, the actual functions
// are defined below.  
//
// You can change the names, but not 
// the signatures (void return and their respective parameters).

// callbacks for the bookends
void setEndsColorCB(vargb::ColorSettings * set_to);
void endsScheduleCompleteCB(vargb::Schedule * sched);

// callback for the center RGB LED
void setCenterColorCB(vargb::ColorSettings * set_to);
// we won't use a schedule end callback for the center light, we just 
// let it rotate as it will by default.



// with our callbacks recognizable by the compiler as functions of the 
// correct types, we create the two VaRGB driver objects:
vargb::VaRGB Ends_leds_prog = vargb::VaRGB(setEndsColorCB, endsScheduleCompleteCB);
vargb::VaRGB Center_led_prog = vargb::VaRGB(setCenterColorCB);



/* *** Schedules *** */


// Now we need some RGB-variation schedules to play with (at least one, anyhow).
//
// Each schedule encapsulates a series of transitions between R-G-B settings,
// that will run in the order they are added to the schedule (below).  
//
// Using multiple schedules allows you to package related curves (transition
// logic) together so you can easily switch between them.
//
// So now, we create the schedule objects, as pointers just because everything
// "below" the VaRGB object expects pointers and it makes the code clearer to use
// 
//   my_pointer
// than
//   &my_object
// especially with arrays (I find &(some_array[0]) too verbose, though you can 
// use that if you like).

#define number_of_ends_schedules  2
vargb::Schedule * ends_schedules[number_of_ends_schedules] = {
  new vargb::Schedule(),
  new vargb::Schedule(),

};

vargb::Schedule * center_schedule = new vargb::Schedule();




/* *** Curves -- the RGB transition specifications *** */

// As mentioned, a schedule is comprised of one or more curves.  These curves 
// describe the behaviour of the RGB settings with time.
//
// There are a few kinds of curves available with the library, like:
//
//  * Linear -- each RGB component varies linearly with time (a constant 
//              slope), like a simple "fade" in or out
//  
//  * Sine -- RGB components vary as sine waves, so quickly around their 
//              mid-point and slower at the extremes
//
//  * Flasher -- RGB components are alternated between 0 and their setting
//
// In addition, there are "combo" curves which allow you to combine 
// other curves with AND or OR logic.  We'll use an AND combo in the final curve
// for the center LED

// now, let's create a few of these curves...

// a single "white" flasher, for schedule 0
#define num_flashes      20
#define time_flashing    6
vargb::Curve::Flasher * flasher_curve = new vargb::Curve::Flasher(VaRGB_COLOR_MAXVALUE, 
VaRGB_COLOR_MAXVALUE, VaRGB_COLOR_MAXVALUE, 
time_flashing, num_flashes);

// a green-tinted sine wave, for schedule 0: say 5 cycles of 2 seconds each 
// (total run time of 10 seconds)
#define sine_runtime             4
#define sinewave_seconds_per_cycle   1
vargb::Curve::Sine * green_sine_wave = new vargb::Curve::Sine(250, VaRGB_COLOR_MAXVALUE, 250, 
sine_runtime
, sinewave_seconds_per_cycle);



// a sequence of linear "fades" for schedule 2, store in an array for convenience
#define num_curves_schedule1  5
vargb::Curve::Linear * schedule1_curves[num_curves_schedule1] = {

  // start black
  new vargb::Curve::Linear(0,0,0,       0), 
  // go to ~1/2 red, over one 5 seconds
  new vargb::Curve::Linear(500,0,0,     5), 
  // go to red+blue, over 2s
  new vargb::Curve::Linear(1000,0,1000, 2), 
  // back down to ~1/2 red over 2s
  new vargb::Curve::Linear(500,0,0,     2), 
  // fade to black for 5s
  new vargb::Curve::Linear(0,0,0,       5), 
};



// Curves for the center LED... we'll do a little dance that's a little more complex, so we can 
// really see the difference between the bookends and the center LED

#define color_max_val 1023

#define center_sine_runtime		6
#define center_sine_secondspercycle	2

#define fade_flash_time		5
#define fade_flash_numflashes	30

#define num_center_curves	12


// A few more complex curves we'll prep here

// sine + blue fade in

// 
vargb::Curve::Sine * center_sine = new vargb::Curve::Sine(500, color_max_val, 0,
                                      center_sine_runtime, 
                                      center_sine_secondspercycle);

vargb::Curve::Linear * fade_in_blue = new vargb::Curve::Linear(0,0, color_max_val, center_sine_runtime);


vargb::Curve::OrLogic * sine_and_blue_fadein = new vargb::Curve::OrLogic(fade_in_blue, center_sine);





// flash and fade-out, a more complex curve we'll prep here...

// fade to black, in fade_flash_time seconds
vargb::Curve::Linear * fade_to_black = new vargb::Curve::Linear(0,0,0, fade_flash_time);
// and the actual flashing on and off
vargb::Curve::Flasher * flash_white = new vargb::Curve::Flasher(color_max_val, color_max_val, color_max_val,
fade_flash_time, fade_flash_numflashes);

// together at last--combined in an AndLogic (output will be the binary ANDed value)
vargb::Curve::AndLogic * flash_and_fade = new vargb::Curve::AndLogic(flash_white, fade_to_black);




vargb::Curve::Curve * center_curves[num_center_curves] = {

  // start black
  new vargb::Curve::Linear(0,0,0,       0), 
  // go to red, over one 2 seconds
  new vargb::Curve::Linear(color_max_val,0,0,     2),
  // stay there for one second
  new vargb::Curve::Linear(color_max_val,0,0,     1),


  // go to green (from red), over one 2 seconds
  new vargb::Curve::Linear(0,color_max_val,0,     2),
  // stay there for one second
  new vargb::Curve::Linear(0,color_max_val,0,     1),

  // go to blue (from green), over one 2 seconds
  new vargb::Curve::Linear(0,0,color_max_val,     2),
  // stay there for one second
  new vargb::Curve::Linear(0,0,color_max_val,     1),

  // go to black instantaneously (0 secs)
  new vargb::Curve::Linear(0,0,0,     0),


  // sine red+green for a bit (all values will oscillate between 0 and max
  // following a sine wave curve)
  sine_and_blue_fadein,


  // go to full white, instantaneously (0 seconds)
  new vargb::Curve::Linear(color_max_val, color_max_val, 
  color_max_val,  0),


  // our fade-flash combo grand finale:
  flash_and_fade,

  // stay at black for a few seconds
  new vargb::Curve::Linear(0,0,0,     2),
  
};






/* *** Callbacks *** */

// these are the callbacks used by the VaRGB driver, that we promised we would
// be defining above (the forward declarations)


void endsScheduleCompleteCB(vargb::Schedule * sched)
{
  // when a schedule has completely run through all of its transitions,
  // this callback is called, with the just-terminated schedule as 
  // the parameter.

  // use this function to do whatever you like, e.g. change the schedule
  // as we're doing here.

  // one important thing is to call resetTicks() if you just want to start
  // from the begining (of this or another schedule):
  Ends_leds_prog.resetTicks();


  // in this case, we'll move on to the next schedule
  byte next_id = sched->id() + 1;
  if (next_id >= number_of_ends_schedules )
  {
    next_id = 0; // past end, so loop back to first schedule...
  }
  Ends_leds_prog.setSchedule(ends_schedules[next_id]);

}


void setEndsColorCB(vargb::ColorSettings * set_to)
{
  // we want to set the color of each of the two side LEDs
  // according to this value, while leaving the center value
  // alone.

  // since we need to push 3 colors down the chain of megabrites
  // to do so, we'll first get the value of the center module

  // make a local copy of the packet for the center value:
  BritePacket current_center_value = *((BritePacket*)(Brite_chain.getState(megabrite_center_idx))); 


  // construct a packet for the color setting of the end modules
  BritePacket ends_value = Brite_chain.colorPacket(set_to->red, set_to->green, set_to->blue);


  // send the update
  Brite_chain.beginUpdate();

  // push the last ("right") end value
  Brite_chain.sendPacket(ends_value);

  // now the center value:
  Brite_chain.sendPacket(current_center_value);

  // now the left end value:
  Brite_chain.sendPacket(ends_value);

  // let the update take effect
  Brite_chain.endUpdate();



#ifdef DEBUG_TO_SERIAL
  // Some debug output
  Serial.print("E:");
  Serial.print(set_to->red, DEC);
  Serial.print(",");
  Serial.print(set_to->green, DEC);
  Serial.print(",");
  Serial.println(set_to->blue, DEC);
#endif

}



void setCenterColorCB(vargb::ColorSettings * set_to)
{
  // same as the above setEndsColorCB, except that the color we'll 
  // be modifying is the value of the center module, while we'll 
  // be preserving the two ends

  // make a local copy of the packet for one of the ends (both ends
  // are identical):
  BritePacket current_ends_value = *((BritePacket*)(Brite_chain.getState(megabrite_left_idx)));


  // construct a packet for the color setting of the center module
  BritePacket center_value = Brite_chain.colorPacket(set_to->red, set_to->green, set_to->blue);



  // send the update
  Brite_chain.beginUpdate();

  // push the last ("right") end value
  Brite_chain.sendPacket(current_ends_value);

  // now the center value:
  Brite_chain.sendPacket(center_value);

  // now the left end value:
  Brite_chain.sendPacket(current_ends_value);

  // let the update take effect
  Brite_chain.endUpdate();



#ifdef DEBUG_TO_SERIAL
  // Some debug output
  Serial.print("              C:");
  Serial.print(set_to->red, DEC);
  Serial.print(",");
  Serial.print(set_to->green, DEC);
  Serial.print(",");
  Serial.println(set_to->blue, DEC);
#endif
  
}








/* *** Arduino functions *** */


// here are the standard Arduino functions, setup() to do just that
// and loop() which will be called repeatedly for ever and ever.


void setup()
{



#ifdef DEBUG_TO_SERIAL
  Serial.begin(serial_baud_rate);
#endif

  // Tell the schedules what curves each is responsible for:



  // First off, the schedules for the RGB LEDs on the ends:

  // add the flasher to the first schedule (0):
  ends_schedules[0]->addTransition(flasher_curve);
  // then add the sinewave here too
  ends_schedules[0]->addTransition(green_sine_wave);

  // add all the linear to the second program
  for (byte i=0; i< num_curves_schedule1; i++)
  {
    ends_schedules[1]->addTransition(schedule1_curves[i]);
  }


  // Finally, tell our VaRGB driver where to begin (schedule 0)
  Ends_leds_prog.setSchedule(ends_schedules[0]);





  // Now, the schedules for the center MegaBrite:
  for (byte i=0; i<num_center_curves; i++)
  {
    center_schedule->addTransition(center_curves[i]);
  }
  // tell the center LED VaRGB driver which schedule to use:
  Center_led_prog.setSchedule(center_schedule);


  // two little TODOs for the TinyBrite: tell it which pins
  // the chain is setup on, and tell it to keep track of state
  // because we're using getState() in the color-setting callbacks:

  // pin setup
  Brite_chain.setup(brite_datapin, brite_clockpin, brite_latchpin);

  // state tracking "on"
  Brite_chain.setStateTracking(true);



}




void loop()
{
  // see the main loop() description in the "Basics" example for details.  In 
  // essence, we need to tick all our VaRGB drivers and wait around a bit.

  // here, we do it "right" by ensuring we delay the correct amount

  unsigned long time_spent_putzing_around_ms = 0;
  unsigned long start_time = millis();
  Ends_leds_prog.tick();
  Center_led_prog.tick();



  // do anything you want to do here...





  // figure out if we spent any measurable time doing stuff...
  unsigned long end_time = millis();

  if (end_time > start_time)
  {
    time_spent_putzing_around_ms = end_time - start_time;
  }


  // the delay time is constant for all the instances of VaRGB and NOT additive
  // so you need only wait "once"
  if (time_spent_putzing_around_ms < Center_led_prog.tickDelayTimeMs())
  {
    delay(Center_led_prog.tickDelayTimeMs() - time_spent_putzing_around_ms);
  }


}

// and they lived happily ever after. the end.
// PatD


