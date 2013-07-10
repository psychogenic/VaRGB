/*

 VaRGBPlatform.cpp --  platform-specific functions, part of the VaRGB library.
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

*/
#include "includes/VaRGBConfig.h"
#include "includes/VaRGBPlatform.h"

namespace vargb {

#ifdef VaRGB_TARGET_PLATFORM_ARDUINO
void delayMs(unsigned long ms) {

	delay(ms);
}
#endif


};
