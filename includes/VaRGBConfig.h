/*

 VaRGBConfig.h -- part of the VaRGB library.
 Copyright (C) 2013 Pat Deegan.

 http://www.flyingcarsandstuff.com/projects/vargb/

 Created on: 2013-03-03

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 See file LICENSE.txt for further informations on licensing terms.


 *****************************  OVERVIEW  *****************************

 Compile-time defines and directives are found here.

*/

#ifndef VARGBCONFIG_H_
#define VARGBCONFIG_H_



/*
 * VaRGB_ENABLE_CURVE_XXX
 *
 * Define the corresponding value for any and
 * all curve types you'd like to use within your code.
 *
 * These default to having all available curves enabled,
 * but are available for cases where you only need a
 * subset and are attempting to save space.
 *
 * Currently available values are:
 *
 * 	VaRGB_ENABLE_CURVE_LINEAR
 * 	Linear progression from start colors to target colors.
 *
 * 	VaRGB_ENABLE_CURVE_FLASHER
 * 	An on-off square wave, of configurable frequency.
 *
 * 	VaRGB_ENABLE_CURVE_SINE
 * 	A sine wave, of configurable frequency, going from all-off to
 * 	target colors at peak.
 * 	Will cost you about ~2k of flash.
 *
 * 	VaRGB_ENABLE_CURVE_LOGICAL
 * 	Various means to combine two curves, e.g. using the Flasher
 * 	and a Sine, combined in an OrLogic you can have
 * 	red and blue flashing, while green oscillates it's intensity
 * 	following a sine wave.  Or you could AndLogic a green flasher
 * 	with that same sine wave to have a pulse of varying intensity.
 * 	Will cost you about ~1k of ROM to enable the combos.
 *
 *
 *  Enabled by default when you define
 *  VaRGB_ENABLE_CURVE_LOGICAL:
 *   AND, OR, NOT, Shift and Threshold
 *
 *
 */
// VaRGB_ENABLE_CURVE_LINEAR -- linear progression
#define VaRGB_ENABLE_CURVE_LINEAR

// VaRGB_ENABLE_CURVE_FLASHER -- on/off flasher
#define VaRGB_ENABLE_CURVE_FLASHER

// VaRGB_ENABLE_CURVE_SINE -- sine waves
//
#define VaRGB_ENABLE_CURVE_SINE

// VaRGB_ENABLE_CURVE_LOGICAL -- curve logic/combinations
//
#define VaRGB_ENABLE_CURVE_LOGICAL

// logical/combo instances:
#ifdef VaRGB_ENABLE_CURVE_LOGICAL
// here we assume that if you enable logical curves, you want
// them all:
#define VaRGB_ENABLE_CURVE_ORLOGIC
#define VaRGB_ENABLE_CURVE_ANDLOGIC
#define VaRGB_ENABLE_CURVE_NOTLOGIC
#define VaRGB_ENABLE_CURVE_SHIFTLOGIC
#define VaRGB_ENABLE_CURVE_THRESHOLDLOGIC
#endif




/*
 * VaRGB_DRIVER_PLATFORM_XXX
 *
 * Define ONE of the available VaRGB_DRIVER_PLATFORM_XXX values to use the specified
 * platform.
 *
 * Certain platforms may require additional configuration, such as VaRGB_DRIVER_PLATFORM_GENERIC.
 * See below for details.
 *
 * Available platform flags are:
 *
 * 	VaRGB_DRIVER_PLATFORM_TINYBRITE
 * 	Use the TinyBrite library to drive the *Brite RGB leds
 *
 *
 * 	VaRGB_DRIVER_PLATFORM_GENERIC
 * 	Use this to control the RGB LEDs in any way, but you'll need
 * 	to specify a few details below.  Default settings would
 * 	work with TinyBrite, anyway.
 *
 *
 */
//
#define VaRGB_DRIVER_PLATFORM_GENERIC
// #define VaRGB_DRIVER_PLATFORM_TINYBRITE









/*
 * VaRGB_DRIVER_PLATFORM_GENERIC additional settings.
 *
 * If you have defined VaRGB_DRIVER_PLATFORM_GENERIC, above,
 * you should make sure the values here are correct.
 *
 * VaRGB_GENERIC_PLAT_COLORVALUE_TYPE -- the type to use for color values (e.g. unsigned int)
 *
 * VaRGB_GENERIC_PLAT_COLORVALUE_MAX -- the maximum possible value for a color (e.g. 1023 for 10-bit values).
 *
 */
#ifdef VaRGB_DRIVER_PLATFORM_GENERIC
#define VaRGB_GENERIC_PLAT_COLORVALUE_TYPE		unsigned int
#define VaRGB_GENERIC_PLAT_COLORVALUE_MAX		1023
#endif







/*
 * VaRGB_DELAY_BETWEEN_UPDATES_MS
 *
 * The delay to use between "ticks" (in millis).
 *
 * Smaller leads to finer grained transitions, but too small
 * is a bad idea.  Tested down to 10 ms, 20 works well.
 *
 */
#define VaRGB_DELAY_BETWEEN_UPDATES_MS			20



#define VaRGB_NUM_UPDATES_PER_SECOND			(1000/VaRGB_DELAY_BETWEEN_UPDATES_MS)
#define VaRGB_SECONDS_TO_UNITTIME(s)			(s * VaRGB_NUM_UPDATES_PER_SECOND)



/*
 * VaRGB_CLASS_DESTRUCTORS_ENABLE
 *
 * Working on an embedded platform, we can normally
 * bypass the need for (often virtual) destructors
 * and save a little space.
 *
 * These are disabled by default, to enable: define
 * VaRGB_CLASS_DESTRUCTORS_ENABLE.
 */
//#define VaRGB_CLASS_DESTRUCTORS_ENABLE




/*
 * VaRGB_TARGET_PLATFORM_XXX
 *
 * Define one (only one) of the available target platform flags,
 * which specifies the mcu on which you are running.
 *
 * Available flags are:
 *
 * VaRGB_TARGET_PLATFORM_ARDUINO
 * For Arduinos.
 *
 * And that's it for now.
 */
#define VaRGB_TARGET_PLATFORM_ARDUINO





//#define VaRGB_DEBUG_ARDUINO

#ifdef VaRGB_DEBUG_ARDUINO

#define DEBUG_OUT2LN(msg, msg2) Serial.print(msg); Serial.println(msg2);
#define DEBUG_OUTLN(msg) Serial.println(msg);
#define DEBUG_OUT(msg)  Serial.print(msg);
#define DEBUG_OUT2(msg, msg2)  Serial.print(msg);  Serial.print(msg2);
#else
#define DEBUG_OUTLN(msg) ;
#define DEBUG_OUT2LN(msg, msg2)	;
#define DEBUG_OUT(msg) ;
#define DEBUG_OUT2(msg, msg2)  ;

#endif

#endif /* VARGBCONFIG_H_ */
