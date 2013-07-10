/*

 vaRGBCurves.h  -- Curve includes, part of the VaRGB library.
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

 This file simply includes the headers for all the Curves configured as
 "enabled" in the VaRGBConfig.h file.

 Your code needs only:

 	 #include <VaRGB.h>
 	 #include <VaRGBCurves.h>

 to have everything available during programming.
*/

#ifndef VARGBCURVES_H_
#define VARGBCURVES_H_
#include "includes/VaRGBConfig.h"

// always include Constant curve, just 'cause
#include "includes/Curves/Constant.h"


// Linear progression
#ifdef VaRGB_ENABLE_CURVE_LINEAR
#include "includes/Curves/Linear.h"
#endif


// 50% duty cycle pulse
#ifdef VaRGB_ENABLE_CURVE_FLASHER
#include "includes/Curves/Flasher.h"
#endif

// sine waves
#ifdef VaRGB_ENABLE_CURVE_SINE
#include "includes/Curves/Sine.h"
#endif


// Logic/Combo curves
#ifdef VaRGB_ENABLE_CURVE_LOGICAL
#include "includes/Curves/Logic.h"
	#ifdef VaRGB_ENABLE_CURVE_ANDLOGIC
	#include "includes/Curves/AndLogic.h"
	#endif

	#ifdef VaRGB_ENABLE_CURVE_ORLOGIC
	#include "includes/Curves/OrLogic.h"
	#endif

	#ifdef VaRGB_ENABLE_CURVE_NOTLOGIC
	#include "includes/Curves/Not.h"
	#endif

	#ifdef VaRGB_ENABLE_CURVE_SHIFTLOGIC
	#include "includes/Curves/Shift.h"
	#endif

	#ifdef VaRGB_ENABLE_CURVE_THRESHOLDLOGIC
	#include "includes/Curves/Threshold.h"
	#endif

#endif


#endif /* VARGBCURVES_H_ */
