/*

 Linear.cpp --  Linear curve implementation, part of the VaRGB library.
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

*/
#include "includes/VaRGBConfig.h"

#ifdef VaRGB_ENABLE_CURVE_LINEAR

#include "includes/Curves/Linear.h"

//#define FLOATIFY_DIVISIONS
namespace vargb {
namespace Curve {




Linear::Linear(VaRGBColorValue target_red, VaRGBColorValue target_green,
		VaRGBColorValue target_blue, VaRGBTimeValue trans_time_seconds) :
		Curve(target_red, target_green, target_blue, trans_time_seconds) {


}

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
Linear::~Linear() {
	// TODO Auto-generated destructor stub
}
#endif

void Linear::setTick(VaRGBTimeValue setTo, IlluminationSettings* initial_settings)
{
	DEBUG_OUT2LN("Linear::setTick ", setTo);

	uint16_t num_updates;

	resetCurrentSettings(initial_settings);

	IlluminationTarget start_target(current_settings.values[vargb_red_idx],
			current_settings.values[vargb_green_idx],
			current_settings.values[vargb_blue_idx],
			0);


	calcDelta(&start_target, &curve_target);


	if (setTo < 1)
	{
		return;
	}


	// ok, now increment all as required
	for (uint8_t i=0; i < VaRGB_NUM_COLORS; i++)
	{
#ifdef FLOATIFY_DIVISIONS
		num_updates = floor( (1.0 * setTo)/delta.delays[i]);
#else

		num_updates = setTo/delta.delays[i];
#endif
		current_settings.values[i] += (num_updates * delta.increments[i]);
	}

	tick_count = setTo;
	settings_req_update = true;

}

void Linear::tick(uint8_t num)
{


	for (uint8_t i=0; i<num; i++)
	{
		tick_count++;

		if (tick_count >= curve_target.transition_ticks)
		{
			curve_completed = true;
		}

		for (uint8_t c=0; c< VaRGB_NUM_COLORS; c++)
		{
			if (delta.increments[c] != 0)
			{
				if (delta.delays[c]==1 || (tick_count % delta.delays[c]) == 0)
				{

					DEBUG_OUTLN("linear color update");
					current_settings.values[c] += delta.increments[c];
					settings_req_update = true;
				}
			}
		}

	}
}


void Linear::calcDelta(IlluminationTarget* start_target, IlluminationTarget* end_target) {
	int real_delta;
	VaRGBColorValue test_delta_per_unittime;
	VaRGBColorValue test_result;
	VaRGBColorValue abs_delta;
	VaRGBTimeValue max_delay_test;
	VaRGBTimeValue test_unittime;
	VaRGBTimeValue best_unittime;
	VaRGBColorValue best_error;
	VaRGBColorValue cur_error;
	uint16_t numslices;

	DEBUG_OUTLN("Linear::calcDelta()");



	for (uint8_t i = 0; i < VaRGB_NUM_COLORS; i++) {

		// Doing color i
		real_delta = end_target->values[i] - start_target->values[i];

		if (real_delta == 0) {
			DEBUG_OUT2LN("No change for color ", i);
			// no change for this color... set a sane update time/amount and skip it.
			delta.delays[i] =
					VaRGB_SECONDS_TO_UNITTIME(VaRGB_MAXIMUM_UPDATE_DELAY_SECONDS);
			delta.increments[i] = 0;
			continue;
		}


		// have an actual difference between start and target, for this color.
		// we'll try to find the best combination of update interval and
		// inc/decrement value (per interval)

		abs_delta = real_delta > 0 ? real_delta : (-1*real_delta);



		if (abs_delta >= end_target->transition_ticks)
		{
			// we'll need to increment often, by 1 or more
			max_delay_test = 3;

		} else {
			// we want to update as often as we can, so by the smallest increment possible
			// the question: how often do we need to go up by 1 to reach abs_delta
			max_delay_test = (end_target->transition_ticks/abs_delta) + 3;
		}

		DEBUG_OUT2LN("abs() delta:", abs_delta);

		DEBUG_OUT2LN("Transition ticks:", end_target->transition_ticks);
		DEBUG_OUT2LN("MAX DELAY TEST:", max_delay_test);


		best_unittime = 1;
		test_unittime = 1;
		best_error = abs_delta; // set it to a "high" value

		// we try a few different delay multiples
		while (best_error > 0 && (test_unittime <= end_target->transition_ticks)
				&& (test_unittime <= max_delay_test)) {
			// assume increment is our unit time, by how much would we need to change value
			// per unit?

#ifdef FLOATIFY_DIVISIONS
			uint16_t numslices = floor(
					(1.0 * end_target->transition_ticks)
							/ (1.0 * test_unittime));


			test_delta_per_unittime = floor(
					(1.0 * abs_delta) / (1.0 * numslices));
#else

			numslices = end_target->transition_ticks/test_unittime;


			test_delta_per_unittime =  abs_delta/numslices;

#endif
			DEBUG_OUT2LN("NUMSLICES:", numslices);
			DEBUG_OUT2LN("test delta:", test_delta_per_unittime);


			// see where we'd be after all time has elapsed
			test_result = test_delta_per_unittime * numslices;
			cur_error = (abs_delta > test_result) ?
							abs_delta - test_result : test_result - abs_delta;

			if (cur_error < best_error) {
				// smaller error here, set as best set

				DEBUG_OUT2LN("BETTER:", cur_error);
				best_error = cur_error;
				best_unittime = test_unittime;
			}

			test_unittime++;
		}
		// now, we should have a decent value for our increment
		delta.delays[i] = best_unittime;

#ifdef FLOATIFY_DIVISIONS

		numslices = (1.0 * end_target->transition_ticks)/(1.0 * best_unittime);
		delta.increments[i] = floor((1.0 * abs_delta)/ (1.0 * numslices);
#else


		numslices = end_target->transition_ticks/best_unittime;
		delta.increments[i] = (abs_delta/numslices);
#endif
		if (real_delta < 0)
		{
			delta.increments[i] *= -1;
		}

		DEBUG_OUTLN("Linear delta calc done");
		DEBUG_OUT("increment: ");
		DEBUG_OUT(delta.increments[i]);
		DEBUG_OUT(" every (ticks):");
		DEBUG_OUTLN(delta.delays[i]);


	}
}

} /* namespace Curve */
} /* namespace vargb */

// VaRGB_ENABLE_CURVE_LINEAR
#endif

