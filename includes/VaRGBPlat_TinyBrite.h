/*

 VaRGBPlat_TinyBrite.h -- part of the VaRGB library.
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


 *****************************  OVERVIEW  *****************************

 TinyBrite-specific platform, for using VaRGB with TinyBrite (ShiftBrite/MegaBrite
 RGB LED driver lib).

 Only enables if VaRGBConfig.h specifies VaRGB_DRIVER_PLATFORM_TINYBRITE as the
 platform.

 NOTE: You need to have the TinyBrite library setup in your environment for this
 to compile.

*/
#ifndef VARGBPLAT_TINYBRITE_H_
#define VARGBPLAT_TINYBRITE_H_

#include <TinyBrite.h>

#define VaRGB_COLOR_MAXVALUE	TINYBRITE_COLOR_MAXVALUE

namespace vargb {

typedef TinyBriteColorValue		VaRGBColorValue;


} /* end namespace vargb */


#endif /* VARGBPLAT_TINYBRITE_H_ */
