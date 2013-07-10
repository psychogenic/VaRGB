/*

 Constant.h -- part of the VaRGB library.
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


*/
#ifndef CONSTANTCURVE_H_
#define CONSTANTCURVE_H_

#include "../Curve.h"

namespace vargb {
namespace Curve {



class Constant : public Curve {
public:
	Constant(VaRGBColorValue target_red, VaRGBColorValue target_green,
			VaRGBColorValue target_blue, VaRGBTimeValue trans_time_seconds);

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
	virtual ~Constant();
#endif


	virtual void setTick(VaRGBTimeValue setTo,  IlluminationSettings* initial_settings=NULL);
	virtual void tick(uint8_t num=1);
private:

};

} /* namespace Curve */
} /* namespace vargb */
#endif /* CONSTANTCURVE_H_ */
