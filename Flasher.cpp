/*

 Flasher.cpp --  Flasher curve implementation, part of the VaRGB library.
 Copyright (C) 2013 Pat Deegan.

 http://www.flyingcarsandstuff.com/projects/vargb/

 Created on: 2013-03-04

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


#ifdef VaRGB_ENABLE_CURVE_FLASHER


#include "includes/Curves/Flasher.h"

// #define FLOATIFY_DIVISIONS
namespace vargb {
namespace Curve {




Flasher::Flasher(VaRGBColorValue target_red, VaRGBColorValue target_green,
		VaRGBColorValue target_blue, VaRGBTimeValue time_seconds,
		uint8_t numFlashes) :
		Curve(target_red, target_green, target_blue, time_seconds),
		num_flashes(numFlashes),
		toggle_interval(1),
		is_flashing(false)
{

	if (num_flashes < 1)
	{
		num_flashes = 1;
	}

}

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
Flasher::~Flasher() {
	// TODO Auto-generated destructor stub
}
#endif


void Flasher::setTick(VaRGBTimeValue setTo, IlluminationSettings* initial_settings)
{

	resetCurrentSettings(initial_settings);

	toggle_interval = curve_target.transition_ticks / (2 * num_flashes);
	if (toggle_interval < 1)
	{
		toggle_interval = 1;
	}

	if ((setTo / toggle_interval) % 2 == 0)
	{
		// flash off
		is_flashing = false;
		resetCurrentSettings(&base_settings);
	} else {

		// flash on
		is_flashing = true;
		resetCurrentSettings(&curve_target);

	}

	tick_count = setTo;
	settings_req_update = true;

}

void Flasher::tick(uint8_t num)
{
	for (uint8_t i=0; i<num; i++)
	{
		tick_count++;

		if (tick_count >= curve_target.transition_ticks)
		{
			curve_completed = true;
		}
		if (tick_count % toggle_interval == 0)
		{
			if (is_flashing)
			{
				// is flashing, reset to base
				resetCurrentSettings(&base_settings);
			} else {

				// not flashing, reset to flash on
				resetCurrentSettings(&curve_target);
			}

			is_flashing = !is_flashing;
			settings_req_update = true;

		}
	}

}



} /* namespace Curve */
} /* namespace vargb */


// VaRGB_ENABLE_CURVE_FLASHER
#endif
