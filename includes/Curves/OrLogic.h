/*

 OrLogic.h -- part of the VaRGB library.
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

 A "OrLogic" curve is a variant of Logic curve, which combines two other
 curves using a bit-wise OR on each of the R, G and B components.


*/

#ifndef ORLOGIC_H_
#define ORLOGIC_H_
#include "../VaRGBConfig.h"
#include "Logic.h"

namespace vargb {
namespace Curve {

class OrLogic : public Logic {
public:
	/*
	 * The OrLogic curve constructor, takes pointers to
	 * each of the children curves to merge with logical OR.
	 */
	OrLogic(vargb::Curve::Curve * curve_a,
			vargb::Curve::Curve * curve_b);

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
	virtual ~OrLogic() {}
#endif

protected:
	virtual void childUpdated();

};

} /* namespace Curve */
} /* namespace vargb */
#endif /* ANDLOGIC_H_ */
