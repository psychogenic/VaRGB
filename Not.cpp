/*

 Not.cpp -- Not curve implementation, part of the VaRGB library.
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

#ifdef VaRGB_ENABLE_CURVE_NOTLOGIC

#include "includes/Curves/Not.h"
#include "includes/Curves/Dummy.h"
#ifndef VaRGB_ENABLE_CURVE_LOGICAL
#error "VaRGB_ENABLE_CURVE_NOTLOGIC defined but not VaRGB_ENABLE_CURVE_LOGICAL (see config)"
#endif

namespace vargb {
namespace Curve {

#define channel_idx_red		0
#define channel_idx_green	1
#define channel_idx_blue	2



Not::Not(vargb::Curve::Curve * curve, bool red_channel, bool green_channel, bool blue_channel)
: Logic(curve)
{
	channel_on[channel_idx_red] = red_channel;
	channel_on[channel_idx_green] = green_channel;
	channel_on[channel_idx_blue] = blue_channel;
}

void Not::childUpdated()
{

	for (uint8_t i=0; i<VaRGB_NUM_COLORS; i++)
	{
		if (channel_on[i])
		{
			// need to apply the NOT
			current_settings.values[i] = VaRGB_COLOR_MAXVALUE & (~(curves[0]->currentSettings()->values[i]));
		} else {
			current_settings.values[i] = curves[0]->currentSettings()->values[i];
		}
	}


}
} /* namespace Curve */
} /* namespace vargb */


#endif
