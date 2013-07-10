/*

 PhasedSine.ino -- VaRGB library example usage, for Arduino.
 Copyright (C) 2013 Pat Deegan.  All rights reserved.
 
 http://flyingcarsandstuff.com/projects/vargb/
 
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.
 
 See file LICENSE.txt for further informations on licensing terms.
 
 *****************************  OVERVIEW  *****************************

 This program simply combines three sine waves--one for each RGB channel--
 that are phased relative to each other.

 You can see this program running towards the end of the intro/demo video:
 https://www.youtube.com/watch?v=WACmTDXhlXo

 You can compile the program to output data to the serial port and/or
 to use the TinyBrite library to display everything through a ShiftBrite
 or MegaBrite.

 The USE_TINYBRITE and DEBUG_TO_SERIAL #defines, below, configure which 
 functionality is actually enabled.  If you want to play with the TinyBrite
 output, get the lib at 
 http://flyingcarsandstuff.com/projects/tinybrite/tinybrite-download/

 
 
 *****************************  HW SETUP  *****************************
 Hardware setup when using an actual Shift/MegaBrite to display values.

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



// May produce a bunch of output, so set a high baud rate:
#define SERIAL_BAUD_RATE   115200
/***** NOTE: Make sure your Serial Monitor setting matches the ABOVE *******/




/* *** Includes *** */

#include <VaRGB.h>
#include <VaRGBCurves.h>

// uncomment the USE_TINYBRITE define to use the TinyBrite library and 
// control a MegaBrite
// #define USE_TINYBRITE

// leave the DEBUG_TO_SERIAL define un-commented to have output sent to 
// the serial line to show what's happenning on the R, G and B channels
// 'graphically'.  Comment it out to avoid the overhead if you're using 
// in a real life application.
#define DEBUG_TO_SERIAL




#ifdef USE_TINYBRITE
	// We'll use TinyBrite to set some MegaBrites
	#include <TinyBrite.h>

	// a few useful defines
	#define megabrite_num_modules	1

	// the pins we're using to talk to the 'brites
	#define brite_datapin		3
	#define brite_latchpin		4
	#define brite_clockpin		5
#endif



/* *** Serial Output *** */
#ifdef DEBUG_TO_SERIAL
	#define SERIALCONN         	Serial
	#define SERIAL_BEGIN(val)  	Serial.begin(val)
	#define SERIAL_PRINTLN(msg)	Serial.println(msg)
#else
	#define SERIALCONN         	;
	#define SERIAL_BEGIN(val)  	;
	#define SERIAL_PRINTLN(msg)	;
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
// We need a color-setting callback
// in order to create our VaRGB driver.

void setColorCB(vargb::ColorSettings * set_to);



// with our callback recognizable by the compiler as a function of the 
// correct type, we create the VaRGB driver object:
vargb::VaRGB RGB_prog = vargb::VaRGB(setColorCB);





/* *** Schedules *** */


// Now we need some RGB-variation schedules to play with (at least one, anyhow).
//
// Each schedule encapsulates a series of transitions between R-G-B settings,
// that will run in the order they are added to the schedule (below).  
//
vargb::Schedule sine_schedule;







/* *** Curves -- the RGB transition specifications *** */


#define sine_runtime                 40
#define sinewave_seconds_per_cycle   2


// a standard sine wave, on the red channel
vargb::Curve::Sine red_sine_wave(VaRGB_COLOR_MAXVALUE, 0, 0,
                                   sine_runtime, sinewave_seconds_per_cycle);


// same sine wave params as the red, but on the green channel AND we set an 
// extra parameter to shift the phase of the sine wave by 90 degrees
vargb::Curve::Sine green_sine_wave(0, VaRGB_COLOR_MAXVALUE, 0, 
                                   sine_runtime, sinewave_seconds_per_cycle, 90);



// final sinewave for blue channel, shifted by 180 degrees (basically inverted)
vargb::Curve::Sine blue_sine_wave(0, 0, VaRGB_COLOR_MAXVALUE,
                                   sine_runtime, sinewave_seconds_per_cycle, 180);




// now, we'd like to send all these at the same time, so we combine them all into
// a single logical curve, by doing
//
// ((RED_SINE OR GREEN_SINE) OR BLUE_SINE)


// combine the red and green:
vargb::Curve::OrLogic red_and_green(&red_sine_wave, &green_sine_wave);

// combine those two with the blue:
vargb::Curve::OrLogic all_sines(&red_and_green, &blue_sine_wave);





/* *** Callbacks *** */


#ifdef DEBUG_TO_SERIAL
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
#endif

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


#ifdef DEBUG_TO_SERIAL
  out_color(set_to->red);
  out_color(set_to->green);
  out_color(set_to->blue);
  SERIAL_PRINTLN("");
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
  // first off, we're just outputting to the serial monitor, so
  // lets set that up and say hi!
  SERIAL_BEGIN(SERIAL_BAUD_RATE);
  
  SERIAL_PRINTLN("Startup. Setting up schedules...");
  
  sine_schedule.addTransition(&all_sines);

  RGB_prog.setSchedule(&sine_schedule);


#ifdef USE_TINYBRITE
  // two little TODOs for the TinyBrite: tell it which pins
  // the chain is setup on, and tell it to keep track of state
  // because we're using getState() in the color-setting callbacks:

  // pin setup
  Brite_chain.setup(brite_datapin, brite_clockpin, brite_latchpin);

  // auto-updates "on"
  Brite_chain.setAutoUpdate(true);
#endif


  // Report our success
  SERIAL_PRINTLN("Done!");
  
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



