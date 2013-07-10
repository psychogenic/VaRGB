/*

 Constant.cpp -- Constant curve implementation, part of the VaRGB library.
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

#ifdef VaRGB_ENABLE_CURVE_LINEAR

#include "includes/Curves/Constant.h"

//#define FLOATIFY_DIVISIONS
namespace vargb {
namespace Curve {




Constant::Constant(VaRGBColorValue target_red, VaRGBColorValue target_green,
		VaRGBColorValue target_blue, VaRGBTimeValue trans_time_seconds) :
		Curve(target_red, target_green, target_blue, trans_time_seconds) {


}

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
Constant::~Constant() {
}
#endif

void Constant::setTick(VaRGBTimeValue setTo, IlluminationSettings* initial_settings)
{
	DEBUG_OUT2LN("Constant::setTick ", setTo);

	// ok, now increment all as required
	for (uint8_t i=0; i < VaRGB_NUM_COLORS; i++)
	{
		current_settings.values[i] = curve_target.values[i];
	}

	tick_count = setTo;
	settings_req_update = true;

}

void Constant::tick(uint8_t num)
{

	for (uint8_t c = 0; c < VaRGB_NUM_COLORS; c++) {
		if (current_settings.values[c] != curve_target.values[c]) {
			current_settings.values[c] = curve_target.values[c];
			settings_req_update = true;

		}
	}

	tick_count += num;
	if (tick_count >= curve_target.transition_ticks) {
		curve_completed = true;
	}
}


} /* namespace Curve */
} /* namespace vargb */

// VaRGB_ENABLE_CURVE_LINEAR
#endif

