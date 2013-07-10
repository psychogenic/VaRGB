/*

 AndLogic.h -- part of the VaRGB library.
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


*/
#ifndef ANDLOGIC_H_
#define ANDLOGIC_H_
#include "../VaRGBConfig.h"
#include "Logic.h"

namespace vargb {
namespace Curve {

class AndLogic : public Logic {
public:
	AndLogic(vargb::Curve::Curve * curve_a, vargb::Curve::Curve * curve_b);

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
	virtual ~AndLogic() {}
#endif

protected:
	virtual void childUpdated();

};

} /* namespace Curve */
} /* namespace vargb */
#endif /* ANDLOGIC_H_ */
