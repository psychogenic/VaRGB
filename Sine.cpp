/*

 Sine.cpp -- Sine curve implementation, part of the VaRGB library.
 Copyright (C) 2013 Pat Deegan.

 http://www.flyingcarsandstuff.com/projects/vargb/

 Created on: 2013-03-05

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 See file LICENSE.txt for further informations on licensing terms.

*/

#include "includes/VaRGBConfig.h"

#ifdef VaRGB_ENABLE_CURVE_SINE
#include "includes/Curves/Sine.h"
#include <math.h>



#ifndef TWO_PI
#define TWO_PI 6.283185307179586476925286766559
#endif


namespace vargb {
namespace Curve {




Sine::Sine(VaRGBColorValue target_red, VaRGBColorValue target_green,
		VaRGBColorValue target_blue, VaRGBTimeValue time_seconds,
		uint8_t secs_per_cycle, uint16_t phase_degrees) :
		Curve(target_red, target_green, target_blue, time_seconds),
		ticks_per_cycle(VaRGB_SECONDS_TO_UNITTIME(secs_per_cycle)),
		radians_current(0)
{
	increment_per_tick = TWO_PI / ticks_per_cycle;
	if (phase_degrees < 1)
	{
		phase_adjustment = 0;

	} else {

		phase_adjustment = (TWO_PI * phase_degrees) / 360.00f;
	}

}

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
Sine::~Sine() {
	// TODO Auto-generated destructor stub
}
#endif


void Sine::setTick(VaRGBTimeValue setTo, IlluminationSettings* initial_settings)
{

	resetCurrentSettings(initial_settings);

	if (setTo < 2)
	{
		radians_current = 0;

	} else {

		VaRGBTimeValue remainderTicks = (setTo - 1) % ticks_per_cycle;

		radians_current = increment_per_tick * remainderTicks;

		// small error here, whatev's

	}

	tick_count = setTo;

	tick();


}

void Sine::tick(uint8_t num)
{
	double sinFactor;
	VaRGBTimeValue remainderTicks;

	for (uint8_t i=0; i<num; i++)
	{
		tick_count++;

		if (tick_count >= curve_target.transition_ticks)
		{
			curve_completed = true;
		}
		remainderTicks = tick_count % ticks_per_cycle;
		if (remainderTicks == 0)
		{
			radians_current = 0;
		} else {
			radians_current += increment_per_tick;
		}

		sinFactor = (sin(radians_current + phase_adjustment) + 1.0)/2.0; // between -1 and 1 so we shift up and div by two


		for (uint8_t c=0; c < VaRGB_NUM_COLORS; c++)
		{
			current_settings.values[c] = curve_target.values[c] * sinFactor;
		}
		settings_req_update = true;

	}

}



} /* namespace Curve */
} /* namespace vargb */


// VaRGB_ENABLE_CURVE_SINE
#endif
