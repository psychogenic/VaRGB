/*

 Flasher.h -- part of the VaRGB library.
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

 A flasher curve is a simple pulse train with a 50% duty cycle: it is "on"
 half the time--at the values specified during construction-- and "off" the
 rest of the time, with all values at 0 by default or set to whatever you've
 specified with setBaseIllumination().



*/

#ifndef FLASHER_H_
#define FLASHER_H_

#include "../Curve.h"

namespace vargb {
namespace Curve {


class Flasher : public Curve {
public:
	/*
	 * Flasher constructor.
	 * Pass the target Red, Green and Blue values, along with
	 * the total run time (in seconds) and the number of flashes
	 * to display during that time.
	 */
	Flasher(VaRGBColorValue target_red, VaRGBColorValue target_green,
			VaRGBColorValue target_blue, VaRGBTimeValue time_seconds,
			uint8_t numFlashes);

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
	virtual ~Flasher();
#endif


	/*
	 * setBaseIllumination()
	 *
	 * If you want to toggle between something other than "OFF" (all zeros) and the
	 * targets you specified in the constructor, prepare an IlluminationSettings
	 * struct with the correct values and pass it to the Flasher instance using
	 * setBaseIllumination().
	 */
	void setBaseIllumination(IlluminationSettings setTo) { base_settings = setTo;}


	virtual void setTick(VaRGBTimeValue setTo,  IlluminationSettings* initial_settings=NULL);
	virtual void tick(uint8_t num=1);
private:

	uint8_t num_flashes;
	VaRGBTimeValue toggle_interval;
	bool is_flashing;

	IlluminationSettings base_settings;


};

} /* namespace Curve */
} /* namespace vargb */
#endif /* LINEAR_H_ */
