/*

 AndLogic.cpp -- AndLogic implementation, part of the VaRGB library.
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

#ifdef VaRGB_ENABLE_CURVE_ANDLOGIC

#include "includes/Curves/AndLogic.h"
#ifndef VaRGB_ENABLE_CURVE_LOGICAL
#error "VaRGB_ENABLE_CURVE_LOGIC_AND defined but not VaRGB_ENABLE_CURVE_LOGICAL (see config)"
#endif

namespace vargb {
namespace Curve {

AndLogic::AndLogic(vargb::Curve::Curve * curve_a,
		vargb::Curve::Curve * curve_b)
: Logic(curve_a, curve_b)
{

}

void AndLogic::childUpdated()
{

	for (uint8_t i=0; i<VaRGB_NUM_COLORS; i++)
	{
		current_settings.values[i] =
				curves[0]->currentSettings()->values[i] &
				curves[1]->currentSettings()->values[i];

	}


}
} /* namespace Curve */
} /* namespace vargb */


#endif
