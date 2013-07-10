/*

 Logic.h -- part of the VaRGB library.
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

 Unlike the atomic curves "logical" curves, based on the
 vargb::Curve::Logic class here, don't specify how the red, green
 and blue values change with time.  Instead, these curves act to
 combine or otherwise transform other curves.

 The name "Logic Curve" is mainly historical, as these curves started
 out as simply AND-combined and OR-combined curves.

 Now additional derivatives exist, such as Shift and Threshold, but
 whatevs.

 Logic curves need at least one, or optionally two, curves on which to operate.
 These are stored in the (protected member) curves[].  The methods are constructed
 to always check both children for required updates etc, so in cases where you
 are only operating on a single child curve, the second curve is a static Dummy
 curve, which basically never requires an update and never terminates.
*/

#ifndef LOGIC_H_
#define LOGIC_H_

#include "../VaRGBConfig.h"
#include "../Curve.h"

#include "Dummy.h"

namespace vargb {
namespace Curve {

#define VARGB_CURVE_LOGIC_NUMCURVES		2

class Logic : public vargb::Curve::Curve {
public:
	/*
	 * Logic Curve constructor
	 * Takes one, or two, pointers to other curves on which to operate.
	 */
	Logic(vargb::Curve::Curve * curve_a, vargb::Curve::Curve * curve_b=NULL);

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
	virtual ~Logic() {}
#endif

	virtual bool completed();
	virtual void settingsUpdated();
	virtual void start(IlluminationSettings* initial_settings = NULL);
	virtual void setTick(VaRGBTimeValue setTo,
			IlluminationSettings* initial_settings = NULL);
	virtual void tick(uint8_t num = 1);


	virtual void reset();

protected:
	/*
	 * childUpdated()
	 * This base class will check the children to see if they've been updated
	 * after every tick.  If this happens to be the case, the childUpdated()
	 * method will be called so the Logic curve instance can do its thing.
	 *
	 * This is an abstract method, which you must override in any derived classes.
	 */
	virtual void childUpdated() = 0;

	vargb::Curve::Curve * curves[VARGB_CURVE_LOGIC_NUMCURVES];


private:
	static Dummy dummy_curve;


};

} /* namespace Curve */
} /* namespace vargb */
#endif /* LOGIC_H_ */
