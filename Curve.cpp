/*

 Curve.cpp -- Curve base class implementation, part of the VaRGB library.
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
#include "includes/Curve.h"

namespace vargb {

IlluminationTargetStruct::IlluminationTargetStruct() {
	values[vargb_red_idx] = 0;
	values[vargb_green_idx] = 0;
	values[vargb_blue_idx] = 0;
	transition_ticks = 0;
}

IlluminationTargetStruct::IlluminationTargetStruct(VaRGBColorValue red,
		VaRGBColorValue green, VaRGBColorValue blue,
		VaRGBTimeValue trans_time_seconds) {
	values[vargb_red_idx] = red;
	values[vargb_green_idx] = green;
	values[vargb_blue_idx] = blue;
	transition_ticks = VaRGB_SECONDS_TO_UNITTIME(trans_time_seconds);
}

namespace Curve {

Curve::Curve(VaRGBColorValue target_red, VaRGBColorValue target_green,
		VaRGBColorValue target_blue, VaRGBTimeValue trans_time_seconds) :
		settings_req_update(false), curve_completed(false), tick_count(0),
		curve_target(target_red,
				target_green, target_blue, trans_time_seconds),
				current_settings()
{

}

#ifdef  VaRGB_CLASS_DESTRUCTORS_ENABLE
Curve::~Curve() {

}
#endif

IlluminationTarget Curve::createTarget(VaRGBColorValue red,
		VaRGBColorValue green, VaRGBColorValue blue,
		VaRGBTimeValue trans_time_seconds) {
	return IlluminationTarget(red, green, blue, trans_time_seconds);
}


void Curve::reset()
{

	curve_completed = false;
	tick_count = 0;
}
void Curve::start(IlluminationSettings* initial_settings)
{
	reset();
	this->setTick(0, initial_settings);
}

void Curve::resetCurrentSettings(IlluminationSettings * setTo)
{

	if (setTo)
	{
		current_settings.values[vargb_red_idx] = setTo->values[vargb_red_idx];
		current_settings.values[vargb_green_idx] = setTo->values[vargb_green_idx];
		current_settings.values[vargb_blue_idx] = setTo->values[vargb_blue_idx];
	} else {

		current_settings.values[vargb_red_idx] = 0;
		current_settings.values[vargb_green_idx] = 0;
		current_settings.values[vargb_blue_idx] = 0;
	}

}

} /* namespace Curve */
} /* namespace vargb */
