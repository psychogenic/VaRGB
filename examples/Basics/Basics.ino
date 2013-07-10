/*

 Basics.ino -- Basic walkthrough of VaRGB for Arduino, part of the VaRGB library.
 Copyright (C) 2013 Pat Deegan.

 http://flyingcarsandstuff.com/projects/vargb/


 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 See file LICENSE.txt for further informations on licensing terms.

 *****************************  OVERVIEW  *****************************

 This is an example of using VaRGB with a couple of schedules, each of 
 which goes through a few curves before ending and causing the VaRGB 
 driver to invoke the "Schedule completed" callback--whereby another schedule
 is set.

 NOTE: It doesn't actually include any code to talk to RGB LED hardware, this
 program simply outputs a graphical representation of the value of each R-G-B
 channel, through the serial port.

 Connect using the serial monitor, at the rate specified by SERIAL_BAUD_RATE, 
 below.
*/

// Produces a bunch of output, so set a high baud rate:
#define SERIAL_BAUD_RATE   115200
/***** NOTE: Make sure your Serial Monitor setting matches the ABOVE *******/




/* *** Includes *** */

#include <VaRGB.h>
#include <VaRGBCurves.h>




/* *** Serial Output *** */
// uncomment the define below IF you're building for 
// the DigiSpark.  "regular" Arduino users should just 
// leave this as is.

// #define BUILD_FOR_DIGISPARK
#ifdef BUILD_FOR_DIGISPARK
  #include <DigiUSB.h>
  #define SERIALCONN         DigiUSB
  #define SERIAL_BEGIN(val)  SERIALCONN.begin()
#else
  #define SERIALCONN         Serial
  #define SERIAL_BEGIN(val)  Serial.begin(val)
#endif



/* *** The VaRGB driver object *** */

// callback declarations.
// We need a color-setting callback and a another 
// callback for when a schedule has run its course, 
// in order to create our VaRGB driver.

// these are forward declarations, the actual functions
// are defined below.  You can change the names, but not 
// the signatures (void return and their respective parameters):
void setColorCB(vargb::ColorSettings * set_to);
void scheduleCompleteCB(vargb::Schedule * sched);


// with our callbacks recognizable by the compiler as functions of the 
// correct types, we create the VaRGB driver object:
vargb::VaRGB RGB_prog(setColorCB, scheduleCompleteCB);





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

#define number_of_schedules  2
vargb::Schedule * available_schedules[number_of_schedules] = {
      new vargb::Schedule(),
      new vargb::Schedule(),
  
};






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
// other curves with AND or OR logic.  These are left for another example.

// now, let's create a few of these curves...

// a single "white" flasher, for schedule 0
#define num_flashes      20
#define time_flashing    6
vargb::Curve::Flasher * flasher_curve = new vargb::Curve::Flasher(VaRGB_COLOR_MAXVALUE, 
                                                        VaRGB_COLOR_MAXVALUE, VaRGB_COLOR_MAXVALUE, 
                                                        time_flashing, num_flashes);

// a green-tinted sine wave, for schedule 0: say 5 cycles of 2 seconds each 
// (total run time of 10 seconds)
#define sine_runtime                 6
#define sinewave_seconds_per_cycle   2
vargb::Curve::Sine * green_sine_wave = new vargb::Curve::Sine(500, VaRGB_COLOR_MAXVALUE, 500, 
                                                          sine_runtime, sinewave_seconds_per_cycle);



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








/* *** Callbacks *** */

// these are the callbacks used by the VaRGB driver, that we promised we would
// be defining above (the forward declarations)

void scheduleCompleteCB(vargb::Schedule * sched)
{
  // when a schedule has completely run through all of its transitions,
  // this callback is called, with the just-terminated schedule as 
  // the parameter.
  
  // use this function to do whatever you like, e.g. change the schedule
  // as we're doing here.
  
  // one important thing is to call resetTicks() if you just want to start
  // from the begining (of this or another schedule):
  RGB_prog.resetTicks();
  
  
  // in this case, we'll move on to the next schedule
  byte next_id = sched->id() + 1;
  if (next_id >= number_of_schedules )
  {
    next_id = 0; // past end, so loop back to first schedule...
  }
  RGB_prog.setSchedule(available_schedules[next_id]);
  
}
  
void out_color(vargb::VaRGBColorValue c)
{
  byte i;
  byte amount = c/16;
  byte spaces = (64 - amount)/2;
  for (i=0; i<spaces; i++)
  {
    SERIALCONN.print(" ");
  }

  for (i=0; i<amount; i++)
  {
    SERIALCONN.print("*");
  }

  for (i=0; i<spaces; i++)
  {
    SERIALCONN.print(" ");
  }
}


void setColorCB(vargb::ColorSettings * set_to)
{
  // the color-setting callback, specified in this examples as this
  // "setColorCB()" function, is called whenever there a change
  // to the RGB color settings is required.
  
  // normally, you'd use some code to effect changes in the real 
  // world, e.g. using TinyBrite to set ShifBrite/MegaBrite RGB 
  // LEDs.
  
  // Here, we'll just output the current settings to the serial 
  // monitor


  out_color(set_to->red);
  out_color(set_to->green);
  out_color(set_to->blue);
  SERIALCONN.println();

}




/* *** Arduino functions *** */


// here are the standard Arduino functions, setup() to do just that
// and loop() which will be called repeatedly for ever and ever.


void setup()
{
  // first off, we're just outputting to the serial monitor, so
  // lets set that up and say hi!
  SERIAL_BEGIN(SERIAL_BAUD_RATE);
  
  SERIALCONN.println("Startup. Setting up schedules...");
  
  
  // Tell the schedules what curves each is responsible for:
  
  
  
  // add the flasher to the first schedule (0):
  available_schedules[0]->addTransition(flasher_curve);
  // then add the sinewave here too
  available_schedules[0]->addTransition(green_sine_wave);
  
  
  
  // add all the linear to the second program
  for (byte i=0; i< num_curves_schedule1; i++)
  {
    available_schedules[1]->addTransition(schedule1_curves[i]);
  }
  
  
  // Finally, tell our VaRGB driver where to begin (schedule 0)
  RGB_prog.setSchedule(available_schedules[0]);
  
  
  // Report our success
  SERIALCONN.println("Done!");
  
}




void loop()
{
  
  // in the main loop, you can do any kind of work you like.  For VaRGB, 
  // the thing to remember is that it needs a little time to do its
  // thing--i.e. figure out if it needs to change the settings and call
  // any of its callbacks.
  
  // For this, there are two ways to proceed:
  // call tick() every VaRGB::tickDelayTimeMs() milliseconds OR
  // if you've got no other work to do, just call the combined
  // tickAndDelay() function (as fast as you can), which will delay
  // VaRGB::tickDelayTimeMs() millis every time its called.
  
  // Since we've nothing better to do here, we just:

  RGB_prog.tickAndDelay();
  
  //
  // The other alternative is to call
  // 
  //    RGB_prog.tick();
  //
  // now you do other stuff,
  //
  //    ...
  //
  // and then figure out how many milliseconds have passed:
  //
  //   unsigned long time_left_to_wait_ms = RGB_prog.tickDelayTimeMs() - time_spent_putzing_around_ms;
  //
  // and take a little break for the appropriate amount of time:
  //
  //   delay(time_left_to_wait_ms);
  //
  // Just make sure you don't have time_spent_putzing_around_ms > RGB_prog.tickDelayTimeMs()
  // or you're gonna have a bad time.

  // See the MultiDriver's loop() for an example of the above in real life.
  
}



