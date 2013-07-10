/*

 Threshold.h -- part of the VaRGB library.
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

 A "Threshold" curve is a variant of Logic curve, which operates on one other
 curve.

 It's job is to ensure the value of the curve is above, or below, a certain
 threshold value.  If it is, the value of the child curve is passed unchanged.
 If it does not meet the threshold condition, a (configurable) default value
 is set instead.

 For simplicity, threshold curves operate on all channels (RGB) identically,
 so if you need something fancy, use other logical curves to mask/combine as
 required.


*/

#ifndef THRESHOLDCURVE_H_
#define THRESHOLDCURVE_H_
#include "../VaRGBConfig.h"


#ifdef VaRGB_ENABLE_CURVE_THRESHOLDLOGIC

#include "../Curve.h"
#include "Logic.h"

namespace vargb {
namespace Curve {

typedef enum ThresholdDirEnum {
	ThresholdAbove=0,
	ThresholdBelow
} ThresholdDir ;

class Threshold : public Logic {
public:
	/*
	 * Threshold constructor
	 * The constructor takes a pointer to the curve on which to operate, along with
	 * a color value to use as a threshold and a "direction" on which to apply the
	 * comparison (whether the child curve should be ThresholdAbove or ThresholdBelow
	 * in order to have its value passed).
	 *
	 * If the child is not ThresholdAbove/ThresholdBelow the threshold value, the
	 * default value will be set instead--you may pass this value as a final parameter
	 * to the constructor (set to 0 otherwise).
	 *
	 */
	Threshold(vargb::Curve::Curve * curve, VaRGBColorValue threshold,
			ThresholdDir direction, VaRGBColorValue default_value=0);

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
	virtual ~Threshold() {}
#endif

protected:
	virtual void childUpdated();

private:

	VaRGBColorValue threshold;
	ThresholdDir threshold_dir;
	VaRGBColorValue default_value;


};


} /* namespace Curve */
} /* namespace vargb */

#endif /*  VaRGB_ENABLE_CURVE_THRESHOLDLOGIC */

#endif /* LINEAR_H_ */
