/*

 Not.h -- part of the VaRGB library.
 Copyright (C) 2013 Pat Deegan.

 http://www.flyingcarsandstuff.com/projects/vargb/

 Created on: 2013-03-04

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 See file LICENSE.txt for further informations on licensing terms.


 *****************************  OVERVIEW  *****************************
 A "Not" curve is a variant of Logic curve, which operates on one other
 curve.

 It will invert (apply a bit-wise NOT) every color channel for which
 a TRUE value was passed in the constructor.

*/

#ifndef NOTCURVE_H_
#define NOTCURVE_H_
#include "../VaRGBConfig.h"


#ifdef VaRGB_ENABLE_CURVE_NOTLOGIC

#include "../Curve.h"
#include "Logic.h"


namespace vargb {
namespace Curve {


class Not : public Logic {
public:
	/*
	 * Not Curve constructor
	 * Takes a pointer to the curve to transform, along with optional boolean parameters for each of the
	 * color channels to specify which colors to invert (defaults to all channels NOTed).
	 */
	Not(vargb::Curve::Curve * curve, bool red_channel=true, bool green_channel=true, bool blue_channel=true);

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
	virtual ~NotLogic() {}
#endif

protected:
	virtual void childUpdated();

private:

	bool channel_on[VaRGB_NUM_COLORS];


};


} /* namespace Curve */
} /* namespace vargb */

#endif /*  VaRGB_ENABLE_CURVE_NOTLOGIC */

#endif /* LINEAR_H_ */
