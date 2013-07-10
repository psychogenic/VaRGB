/*

 Sine.h -- part of the VaRGB library.
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

 Sine curves will cycle the R-G-B components from 0 to specified target
 following a sinewave curve. You can specify distinct targets (maximum
 values) for each color channel.

 When constructing, you specify the total run time and the number of
 seconds to spend in one complete cycle (from 1/2 of max, to max, down
 to 0 and back up to 1/2 of max).

 You can optionally adjust the phase of the sine wave using an extra
 parameter in the constructor.



*/

#ifndef SINEWAVE_H_
#define SINEWAVE_H_

#include "../Curve.h"

namespace vargb {
namespace Curve {


class Sine : public Curve {
public:
	/*
	 * Sine curve constructor.
	 * Pass the target (maximum) values for red, green and blue components,
	 * along with the total curve run time (in seconds) and the number of
	 * seconds per (sin) cycle.
	 *
	 * A final, optional, parameter allows you to set the phase of the sine
	 * wave--use a value in degrees, between 0 and 359.
	 */
	Sine(VaRGBColorValue target_red, VaRGBColorValue target_green,
			VaRGBColorValue target_blue, VaRGBTimeValue time_seconds,
			uint8_t seconds_per_cycle, uint16_t phase_degrees=0);

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
	virtual ~Sine();
#endif



	virtual void setTick(VaRGBTimeValue setTo,  IlluminationSettings* initial_settings=NULL);
	virtual void tick(uint8_t num=1);
private:

	VaRGBTimeValue ticks_per_cycle;
	float increment_per_tick;
	float radians_current;
	float phase_adjustment;


	IlluminationSettings base_settings;


};

} /* namespace Curve */
} /* namespace vargb */
#endif /* SINEWAVE_H_ */
