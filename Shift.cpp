/*

 Shift.cpp -- Shift curve implementation, part of the VaRGB library.
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

#ifdef VaRGB_ENABLE_CURVE_SHIFTLOGIC

#include "includes/Curves/Shift.h"

#ifndef VaRGB_ENABLE_CURVE_LOGICAL
#error "VaRGB_ENABLE_CURVE_SHIFTLOGIC defined but not VaRGB_ENABLE_CURVE_LOGICAL (see config)"
#endif

namespace vargb {
namespace Curve {




Shift::Shift(vargb::Curve::Curve * curve, uint8_t bits, ShiftDir direction)
: shift_bits(bits), shift_dir(direction), Logic(curve)
{

}

void Shift::childUpdated()
{

	for (uint8_t i=0; i<VaRGB_NUM_COLORS; i++)
	{
		if (shift_dir == ShiftLeft)
		{
			// need to apply the shift left
			current_settings.values[i] = curves[0]->currentSettings()->values[i] << shift_bits;
		} else {
			current_settings.values[i] = curves[0]->currentSettings()->values[i] >> shift_bits;
		}
	}


}
} /* namespace Curve */
} /* namespace vargb */


#endif
