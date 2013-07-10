/*

 Schedule.cpp -- Schedule implementation, part of the VaRGB library.
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
#include <stdlib.h>
#include <string.h>
#include "includes/VaRGBConfig.h"
#include "includes/Schedule.h"
#include "VaRGB.h"

namespace vargb {

ScheduleID Schedule::schedule_counter = 0;
Schedule::Schedule(ScheduleID id) :
		sched_id(id),
		transition_ptr_list(NULL), transition_index(0),
		transition_num(0), total_schedule_ticks(0),
		driver(NULL)
{

	if (sched_id == 0)
	{
		sched_id = schedule_counter;
	} else {
		schedule_counter = sched_id > schedule_counter ? sched_id: schedule_counter;
	}

	schedule_counter++;

	expandTransitionList();

}

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
Schedule::~Schedule() {

	if (transition_ptr_list) {
		free(transition_ptr_list);
	}
}
#endif

bool Schedule::addTransition(vargb::Curve::Curve * a_curv) {
	if (transition_num >= transition_max) {
		// we need some more space for this item...
		if (!expandTransitionList()) {
			return false;
		}
	}

	transition_ptr_list[transition_num] = a_curv;

	total_schedule_ticks += (a_curv->target())->transition_ticks;

	transition_num++;

	return true;

}

void Schedule::setTick(VaRGBTimeValue tick_count) {
	VaRGBTimeValue ticks_for_transition;
	VaRGBTimeValue tick_position = tick_count % total_schedule_ticks;
	VaRGBTimeValue tick_remainder = 0;

	DEBUG_OUT2LN("Schedule::setTick ", tick_count);
	transition_index = 0;

	while (tick_position > 0) {
			ticks_for_transition =
					(transition_ptr_list[transition_index]->target())->transition_ticks;
			if (tick_position >= ticks_for_transition) {
				tick_position -= ticks_for_transition;
				transition_index++;
			} else {
				tick_remainder = tick_position;
				tick_position = 0;
			}

		}

	// found the transition (curve) we are in
	IlluminationSettings * last_target_settings = NULL;
	if (transition_index > 0) {
		// we should have ended at the last transition's target,
		// so we use that as the initial position
		last_target_settings =
				transition_ptr_list[transition_index - 1]->target();
	}

	// set the current transition's position to whatever
	transition_ptr_list[transition_index]->reset();
	transition_ptr_list[transition_index]->setTick(tick_remainder,
			last_target_settings);


	sendCurTransitionSettings();

}

void Schedule::tick(uint8_t num) {



	transition_ptr_list[transition_index]->tick(num);

	if (transition_ptr_list[transition_index]->settingsNeedUpdate()) {
		// DEBUG_OUTLN("setting need update");
		sendCurTransitionSettings();
	}

	if (transition_ptr_list[transition_index]->completed()) {

		IlluminationTarget * last_target = transition_ptr_list[transition_index]->target();
		DEBUG_OUTLN("transition complete");
		transition_index++;
		if (transition_index >= transition_num) {

			DEBUG_OUTLN("all transitions done (schedule complete)");
			transition_index = 0;
			driver->scheduleComplete(this);

		} else {
			// we move on to the next transition...
			transition_ptr_list[transition_index]->start(last_target);
		}
	}
}
void Schedule::sendCurTransitionSettings() {

	if (transition_num < 1) {
		return;
	}

	IlluminationSettings * cur_illum =
			transition_ptr_list[transition_index]->currentSettings();

	ColorSettings cur_settings;
	cur_settings.red = cur_illum->values[vargb_red_idx];
	cur_settings.green = cur_illum->values[vargb_green_idx];
	cur_settings.blue = cur_illum->values[vargb_blue_idx];

	driver->setColor(this, &cur_settings);

	transition_ptr_list[transition_index]->settingsUpdated();
}

bool Schedule::expandTransitionList(uint8_t by_amount) {

	int curveitm_ptr_size = sizeof(vargb::Curve::Curve *);
	vargb::Curve::Curve ** new_list = NULL;

	if (!by_amount) {
		// set optional param to default value
		by_amount = VaRGB_SCHEDULE_CURVELIST_EXPAND_BYAMOUNT;
	}

	if (transition_ptr_list) {
		// we've already got a list in hand... we want to expand it a bit
		new_list = (vargb::Curve::Curve**) realloc(transition_ptr_list,
				curveitm_ptr_size * (transition_num + by_amount));

	} else {
		// no list yet, malloc one please:
		new_list = (vargb::Curve::Curve**) malloc(
				curveitm_ptr_size * by_amount);

	}

	if (new_list != NULL) {
		// we gots a new list, huzzah.

		// make note of our new space...
		transition_max += by_amount;

		// zero the newly allocated memory
		memset(&(new_list[transition_num]), 0, curveitm_ptr_size * by_amount);

		// keep that pointer
		transition_ptr_list = new_list;

		// success!
		return true;
	}

	// abject failure...
	return false;
}

} /* namespace vargb */
