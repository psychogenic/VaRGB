/*

 Logic.cpp -- Logical curve base class implementation, part of the VaRGB library.
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

#ifdef VaRGB_ENABLE_CURVE_LOGICAL

#include "includes/Curves/Logic.h"


namespace vargb {
namespace Curve {

Dummy Logic::dummy_curve;

Logic::Logic(vargb::Curve::Curve* curve_a,
		vargb::Curve::Curve* curve_b) :
		vargb::Curve::Curve(0,0,0,0)
{
	curves[0] = curve_a;
	if (curve_b == NULL)
	{
		curves[1] = &dummy_curve;
	} else {
		curves[1] = curve_b;
	}
}
bool Logic::completed()
{
	for (uint8_t i = 0; i < VARGB_CURVE_LOGIC_NUMCURVES; i++) {
		if (curves[i]->completed())
		{
			return true;
		}
	}

	return false;
}
void Logic::settingsUpdated() {

	settings_req_update = false;
	for (uint8_t i = 0; i < VARGB_CURVE_LOGIC_NUMCURVES; i++) {
		curves[i]->settingsUpdated();
	}
}

void Logic::start(IlluminationSettings* initial_settings) {

	for (uint8_t i = 0; i < VARGB_CURVE_LOGIC_NUMCURVES; i++) {
		curves[i]->start(initial_settings);
	}
}

void Logic::setTick(VaRGBTimeValue setTo,
		IlluminationSettings* initial_settings) {

	for (uint8_t i = 0; i < VARGB_CURVE_LOGIC_NUMCURVES; i++) {
		curves[i]->setTick(setTo, initial_settings);
		if (curves[i]->settingsNeedUpdate()) {
			settings_req_update = true;
		}

	}

	if (settings_req_update) {
		this->childUpdated();
	}
}

void Logic::tick(uint8_t num) {

	for (uint8_t i = 0; i < VARGB_CURVE_LOGIC_NUMCURVES; i++) {
		curves[i]->tick(num);
		if (curves[i]->settingsNeedUpdate()) {
			settings_req_update = true;
		}

	}

	if (settings_req_update) {
		this->childUpdated();
	}
}

void Logic::reset() {

	for (uint8_t i = 0; i < VARGB_CURVE_LOGIC_NUMCURVES; i++) {
		curves[i]->reset();
	}
}

} /* namespace Curve */
} /* namespace vargb */


#endif


