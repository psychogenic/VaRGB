/*

 Dummy.cpp -- Dummy curve implementation, part of the VaRGB library.
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

*/

#include "includes/VaRGBConfig.h"

#ifdef VaRGB_ENABLE_CURVE_NOTLOGIC

#include "includes/Curves/Dummy.h"


namespace vargb {
namespace Curve {




Dummy::Dummy() :
		Curve(0,0,0,0) {


}

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
Dummy::~Dummy() {
	// TODO Auto-generated destructor stub
}
#endif



} /* namespace Curve */
} /* namespace vargb */

// VaRGB_ENABLE_CURVE_LOGIC_NOT
#endif

