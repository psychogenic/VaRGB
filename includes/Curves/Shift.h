/*

 Shift.h -- part of the VaRGB library.
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

 A "Shift" curve is a variant of Logic curve, which operates on one other
 curve.

 It will shift the child curve component values, either left or right, by
 a configurable amount.

 For example, you could use an AND curve with a Constant to mask out the
 higher level bits of a Linear curve, then shift the resulting curve left,
 to produce a large sawtooth.

 Logically, this would look like


 Shift(

 	 AND(
 	 	 Constant(31,0,0, x),   <-- 31 == 0b000011111 -- so a mask for lower bits

 	 	 Linear(1000,0,0, x) <-- linear progression to 1000 on red

 	 ) <-- AND will result in a series of values < 32, cycling quickly

 	 5, <-- shift by 5 positions
 	 ShiftLeft <-- shift left, so the cycling will be magnified
 )

 And thus we have a nice sawtooth curve on the red channel.



*/

#ifndef SHIFTCURVE_H_
#define SHIFTCURVE_H_
#include "../VaRGBConfig.h"


#ifdef VaRGB_ENABLE_CURVE_SHIFTLOGIC

#include "../Curve.h"
#include "Logic.h"

namespace vargb {
namespace Curve {

typedef enum ShiftDirEnum {
	ShiftRight=0,
	ShiftLeft
} ShiftDir ;

class Shift : public Logic {
public:
	/*
	 * Shift curve constructor
	 * Takes a pointer to the curve to shift, along with the number of bits (or positions) to shift by
	 * and the direction (should be one of vargb::Curve::ShiftLeft or vargb::Curve::ShiftRight).
	 *
	 * Note that, for simplicity, Shift operates on all channels.  If this isn't what you want, use
	 * a AND/Constant combo to mask out other channels as outlined above.
	 */
	Shift(vargb::Curve::Curve * curve, uint8_t bits, ShiftDir direction);

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
	virtual ~Shift() {}
#endif

protected:
	virtual void childUpdated();

private:

	uint8_t shift_bits;
	ShiftDir shift_dir;


};


} /* namespace Curve */
} /* namespace vargb */

#endif /*  VaRGB_ENABLE_CURVE_SHIFTLOGIC */

#endif /* LINEAR_H_ */
