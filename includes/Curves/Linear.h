/*

 Linear.h -- part of the VaRGB library.
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

 A Linear transition curve will go from whichever values are current when
 the curve begins to the target values, in a smooth linear progression over
 the course of the curve's transition time.

 If you want to, for instance, fade out from "white" (all on) to "black"
 (all off) in 12 seconds, you need to be certain you're actually currently
 set to "white" when the curve begins.

 There are a number of ways to do this... You could have a schedule that
 includes a Linear curve that ends at "white"

 	 Linear(max_value, max_value, max_value, num_seconds);

 followed by another Linear that does your fade-out:

 	 Linear(0,0,0, 12);

 Or you could have a constant curve to instantaneously set the white:


 	 Constant(max_value, max_value, max_value, 0);

 Or whatever.  The important thing to remember is that linear curves
 are guaranteed to end at the RGB values you've specified in the
 constructor, but exactly *how* they get there during the transition
 time allowed depends on the starting conditions.


*/

#ifndef LINEAR_H_
#define LINEAR_H_

#include "../Curve.h"

namespace vargb {
namespace Curve {


/*
 * IlluminationDelta
 * Used internally to record how to progress along the linear
 * path, for each color component/channel.
 */
typedef struct IlluminationDeltaStruct {

	int increments[VaRGB_NUM_COLORS];
	VaRGBTimeValue delays[VaRGB_NUM_COLORS];


} IlluminationDelta;


class Linear : public Curve {
public:

	/*
	 * Linear curve constructor.
	 * The parameters are the final values for red, green and blue, along with
	 * the number of seconds within which to arrive there from whichever settings
	 * were current at the start.
	 */
	Linear(VaRGBColorValue target_red, VaRGBColorValue target_green,
			VaRGBColorValue target_blue, VaRGBTimeValue trans_time_seconds);

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
	virtual ~Linear();
#endif


	virtual void setTick(VaRGBTimeValue setTo,  IlluminationSettings* initial_settings=NULL);
	virtual void tick(uint8_t num=1);
private:
	IlluminationDelta delta;


	void calcDelta(IlluminationTarget* start_target, IlluminationTarget* end_target);

};

} /* namespace Curve */
} /* namespace vargb */
#endif /* LINEAR_H_ */
