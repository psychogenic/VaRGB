/*

 VaRGBPlatform.h -- part of the VaRGB library.
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

 The VaRGB platform hides the platform-specific details from upper layers
 and includes relevant headers, as determined by the #defines in
 VaRGBConfig.h.



*/

#ifndef VARGBPLATFORM_H_
#define VARGBPLATFORM_H_

#include <inttypes.h>

#define VaRGB_MAXIMUM_UPDATE_DELAY_SECONDS		11

namespace vargb {


typedef uint16_t VaRGBTimeValue;


// hides VaRGB_TARGET_PLATFORM_* specific details of the delay method
void delayMs(unsigned long ms);



} /* end namespace vargb */

// conditional includes

#ifdef VaRGB_DRIVER_PLATFORM_TINYBRITE
#include "VaRGBPlat_TinyBrite.h"
#endif

#ifdef VaRGB_DRIVER_PLATFORM_GENERIC
#include "VaRGBPlat_Generic.h"
#endif

#ifdef VaRGB_TARGET_PLATFORM_ARDUINO
#include <Arduino.h>
#endif


#ifndef NULL
#define NULL 	0x0
#endif



#endif /* VARGBPLATFORM_H_ */
