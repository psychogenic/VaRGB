/*

 VaRGBPlat_Generic.h -- part of the VaRGB library.
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

 "Generic", all purpose and default, platform configuration.  Uses values
 as specified in the VaRGBConfig.h file, doesn't have any external dependencies
 as is the case with the TinyBrite platform.



*/

#ifndef VARGBPLAT_GENERIC_H_
#define VARGBPLAT_GENERIC_H_

#include "VaRGBConfig.h"

#define VaRGB_COLOR_MAXVALUE	VaRGB_GENERIC_PLAT_COLORVALUE_MAX


namespace vargb {

typedef VaRGB_GENERIC_PLAT_COLORVALUE_TYPE		VaRGBColorValue;


} /* end namespace vargb */


#endif /* VARGBPLAT_TINYBRITE_H_ */
