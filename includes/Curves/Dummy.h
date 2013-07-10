/*

 Dummy.h -- part of the VaRGB library.
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
 A Dummy curves does absolutely nothing.

 It is used by the Logic curve base class for instances where the derived
 implementation class only operates on a single curve, in order to keep the
 code identical for single- (like Not) or dual-curve (like And) operators.

 Dummy curves never require an update and never complete.  They are for
 internal use, please ignore.

*/

#ifndef DUMMYCURVE_H_
#define DUMMYCURVE_H_

#include "../Curve.h"

namespace vargb {
namespace Curve {


/* Dummy curve never completes, never completes, doesn't do anything -- used by "not" combo. */
class Dummy : public Curve {
public:
	Dummy();

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
	virtual ~Dummy();
#endif


	VARGB_CURVE_VIRTORINLINE_METHOD_PREFIX bool completed() { return false; }

	virtual void setTick(VaRGBTimeValue setTo,  IlluminationSettings* initial_settings=NULL) {}
	virtual void tick(uint8_t num=1) {}

private:


};

} /* namespace Curve */
} /* namespace vargb */
#endif /* LINEAR_H_ */
