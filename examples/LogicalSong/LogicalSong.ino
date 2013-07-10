/*

 VaRGB library example usage.
 Copyright (C) 2013 Pat Deegan.  All rights reserved.
 
 http://flyingcarsandstuff.com/projects/vargb/
 
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.
 
 See file LICENSE.txt for further informations on licensing terms.
 
 *****************************  OVERVIEW  *****************************
 
 In this program, we do a lot of relatively convoluted stuff, just to 
 play with the various "logical" transition curves.

 You can see it running at the end of the intro/demo video:
 https://www.youtube.com/watch?v=WACmTDXhlXo

 You can compile the program to output data to the serial port and/or
 to use the TinyBrite library to display everything through a ShiftBrite
 or MegaBrite.

 The USE_TINYBRITE and DEBUG_TO_SERIAL #defines, below, configure which 
 functionality is actually enabled.  If you want to play with the TinyBrite
 output, get the lib at 
 http://flyingcarsandstuff.com/projects/tinybrite/tinybrite-download/

 
 
 *****************************  HW SETUP  *****************************
 Hardware setup when using an actual Shift/MegaBrite to show values.

 The following code expects an Arduino-compatible device tied to a
 chain of three *Brites (can be ShiftBrite, MegaBrite) or anything
 that uses the A6281 PWM LED driver.
 
 Basic hardware setup (power connections/voltage regulation for the
 Arduino are not shown):
 
 
 Arduino                   *Brite device
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

// uncomment the following line to enable use
// of the TinyBrite library (which must be installed)
// Get it at 
// http://flyingcarsandstuff.com/projects/tinybrite/tinybrite-download/

// #define USE_TINYBRITE

#ifdef USE_TINYBRITE
// We'll use TinyBrite to set some MegaBrites
#include <TinyBrite.h>
#endif


// *** DEBUG ***
// uncomment the define below to send
// color adjustment debug info to Serial
//
//

#define DEBUG_TO_SERIAL

#ifndef USE_TINYBRITE
// if we're not using TinyBrite, send output to serial
#ifndef DEBUG_TO_SERIAL
#define DEBUG_TO_SERIAL
#endif
#endif

#define serial_baud_rate	115200


// a few useful defines
#define megabrite_num_modules	1

// the pins we're using to talk to the 'brites
#define brite_datapin		3
#define brite_latchpin		4
#define brite_clockpin		5

#define max_color_val	1023
#define half_color_val	511






#ifdef DEBUG_TO_SERIAL
#define SERIAL_OUT(msg)		Serial.print(msg)
#define SERIAL_OUTLN(msg)	Serial.println(msg)

void out_color(vargb::VaRGBColorValue c)
{
  byte i;
  byte amount = c/16;
  byte spaces = (64 - amount)/2;
  for (i=0; i<spaces; i++)
  {
    Serial.print(" ");
  }

  for (i=0; i<amount; i++)
  {
    Serial.print("*");
  }

  for (i=0; i<spaces; i++)
  {
    Serial.print(" ");
  }
}

#else
#define SERIAL_OUT(msg)		;
#define SERIAL_OUTLN(msg)	;
#endif







#ifdef USE_TINYBRITE
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
#endif



/* *** The VaRGB driver object *** */

// callback declarations.
// We need a color-setting callback in order to create
// our VaRGB driver.

// this is a forward declarations, the actual function
// is defined below.
//
// You can change the name, but not the signature


void setColorCB(vargb::ColorSettings * set_to);


// with our callback recognizable by the compiler as a function of the
// correct type, we create the VaRGB driver object:
vargb::VaRGB logic_driver(setColorCB);



/* *** Schedules *** */


// Now we need at least one RGB-variation schedule to play with.
//
// Each schedule encapsulates a series of transitions between R-G-B settings,
// that will run in the order they are added to the schedule (below).
//
// Using multiple schedules allows you to package related curves (transition
// logic) together so you can easily switch between them.
//

vargb::Schedule transition_schedule;



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
// In addition, there are "logic" curves which allow you to combine
// other curves with AND, OR and NOT logic.
// You can stack logic curves together, for instance taking a sine wave
// then invert a channel (say, red) using a Not and combine with a linear
// curve that has the red channel masked out (using an And), with an Or.
// 
// Logically, this would look like
//
//     Or(And(Linear(...), Constant(...)), Not(Sine(...), true, false, false))


// now, let's create a few of these curves...

// a single simple "white" flasher
#define num_flashes      15
#define time_flashing    3
vargb::Curve::Flasher flasher_curve(max_color_val,
          max_color_val, max_color_val,
          time_flashing, num_flashes);



vargb::Curve::Flasher second_flasher(max_color_val,
          max_color_val, max_color_val,
          time_flashing, num_flashes);
// The Not is created with the curve to apply it to, along with a boolean to denote whether
// each of the RED, GREEN, and BLUE channels are affected.  Here we only NOT the GREEN:
vargb::Curve::Not not_flasher_curve(&second_flasher, false, true, false);





// a sine wave that goes from 0 to max on all channels
#define sine_runtime             		8
#define sinewave_seconds_per_cycle   	        2
vargb::Curve::Sine sine_wave(max_color_val, max_color_val, max_color_val,
                             sine_runtime, sinewave_seconds_per_cycle);

// Now we half-rectify that sine wave by creating a Threshold curve
vargb::Curve::Threshold half_rectified_sine(&sine_wave, 511, vargb::Curve::ThresholdAbove, 512);  




// More rectification, we'll fully rectify the red and blue components of the signal by doing this:

// Create a second sine wave of the same duration and frequency as the first

vargb::Curve::Sine sine_wave_2(max_color_val, max_color_val, max_color_val,
                      sine_runtime, sinewave_seconds_per_cycle);

// Invert the red and blue channels, this will shift the phase of the sine wave on these channels
vargb::Curve::Not inverted_sine(&sine_wave_2, true, false, true);

// create a mask so we'll let through only the red and blue channels
vargb::Curve::Constant redandblue_mask_curve(max_color_val, 0, max_color_val, sine_runtime);

// mask the inverted sine, by AND-ing with the mask
vargb::Curve::AndLogic redandblue_inverted_sine(&redandblue_mask_curve, &inverted_sine);

// half-rectify our red+blue sine signals, using a Threshold
vargb::Curve::Threshold half_rectified_redandbluesine(&redandblue_inverted_sine, 511, vargb::Curve::ThresholdAbove, 512);  

// combine the half-rectified red+blue with the original half-rectified sine wave
vargb::Curve::OrLogic fully_rectified_red_and_blue(&half_rectified_redandbluesine, &half_rectified_sine);





/* grand finale */

// For our grand finale, we'll have three distinct waves for each color channel
#define grand_finale_seconds  22

// green will have a linear sawtooth, a signal that rises steadily to max, then abruptly
// drops to 0:

// create a steady rising linear value on green
vargb::Curve::Linear steady_rising(0, max_color_val, 0, grand_finale_seconds);

// create a mask for some of the lower bits, i.e. 124 == b001111100
vargb::Curve::Constant green_mask_curve(0, 124, 0, grand_finale_seconds);

// apply the mask to the linear green curve
vargb::Curve::AndLogic masked_steady_rising(&steady_rising, &green_mask_curve);

// left-shift the resulting values up, so the signal strength is very visible: 
// yay, we have a sawtooth
vargb::Curve::Shift sawtooth(&masked_steady_rising, 3, vargb::Curve::ShiftLeft);


// for the red channel, we'll have a pseudo-sawtooth, based on a sine signal
// so it will vary the rate at which it changes and the direction of the sawtooth

// create a slowly changing sine wave
vargb::Curve::Sine slow_sine(max_color_val, 0, 0, grand_finale_seconds, 10);

// create a mask for the red channel, 62 == b0000111110, so we keep the lower bits
vargb::Curve::Constant red_mask_curve(62, 0, 0, grand_finale_seconds);

// mask that slow sine wave by AND-ing with our constant value curve
vargb::Curve::AndLogic masked_sine(&slow_sine, &red_mask_curve);

// shift the signal up (left) so we get some visible action
vargb::Curve::Shift alt_sawtooth(&masked_sine, 4, vargb::Curve::ShiftLeft);


// now we have two kinds of sawtooth on the red and green channels
// combine them into one curve, by OR-ing:
vargb::Curve::OrLogic both_sawtooths(&alt_sawtooth, &sawtooth);


// create a quicker sine wave on the blue channel
vargb::Curve::Sine quick_sine(0, 0, max_color_val, grand_finale_seconds, 2);

// combine that quick sine wave with the sawtooths from before
vargb::Curve::OrLogic sawtooths_and_sine(&both_sawtooths, &quick_sine);


// one second break...
vargb::Curve::Constant rest_curve(0,0,0, 1);

// straight to black
vargb::Curve::Linear instant_black(0,0,0,0);




/* *** Callbacks *** */

// these are the callbacks used by the VaRGB driver, that we promised we would
// be defining above (the forward declarations)




void setColorCB(vargb::ColorSettings * set_to)
{


#ifdef DEBUG_TO_SERIAL
  // Some debug output

  out_color(set_to->red);
  out_color(set_to->green);
  out_color(set_to->blue);
  Serial.println();

#endif

#ifdef USE_TINYBRITE
  // send the update
  Brite_chain.sendColor(set_to->red,set_to->green, set_to->blue);
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

  // Tell the schedule what curves it is responsible for:


  // straight flasher:
  transition_schedule.addTransition(&flasher_curve);



  // inverted green flasher:
  transition_schedule.addTransition(&not_flasher_curve);


  // straight sine wave
  transition_schedule.addTransition(&sine_wave);



  // half-rectified sine wave
  // 
  transition_schedule.addTransition(&half_rectified_sine);



  // inverted red and blue sine
  transition_schedule.addTransition(&inverted_sine);

  // fully rectified
  transition_schedule.addTransition(&fully_rectified_red_and_blue);


  // blacken
  transition_schedule.addTransition(&instant_black);


  // wow me, amadeus
  transition_schedule.addTransition(&sawtooths_and_sine);

  // break time
  transition_schedule.addTransition(&rest_curve);


  // Finally, tell our VaRGB driver which schedule to use
  logic_driver.setSchedule(&transition_schedule);


#ifdef USE_TINYBRITE
  // two little TODOs for the TinyBrite: tell it which pins
  // the chain is setup on, and tell it to keep track of state
  // because we're using getState() in the color-setting callbacks:

  // pin setup
  Brite_chain.setup(brite_datapin, brite_clockpin, brite_latchpin);

  // auto-updates "on"
  Brite_chain.setAutoUpdate(true);
#endif



}




void loop()
{
  // see the main loop() description in the "Basics" example for details.  In
  // essence, we need to tick all our VaRGB drivers and wait around a bit.

  // here, we do it "right" by ensuring we delay the correct amount

  unsigned long time_spent_putzing_around_ms = 0;
  unsigned long start_time = millis();
  logic_driver.tick();




  // do anything you want to do here...
  // ...
  // time passes...




  // figure out if we spent any measurable time doing stuff...
  unsigned long end_time = millis();

  if (end_time > start_time)
  {
    time_spent_putzing_around_ms = end_time - start_time;
  }


  // the delay time is constant for all the instances of VaRGB and NOT additive
  // so you need only wait "once"
  if (time_spent_putzing_around_ms < logic_driver.tickDelayTimeMs())
  {
    delay(logic_driver.tickDelayTimeMs() - time_spent_putzing_around_ms);
  }


}

// and they lived happily ever after. the end.
// PatD


