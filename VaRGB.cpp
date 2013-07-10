/*

 VaRGB.cpp -- VaRGB driver implementation, part of the VaRGB library.
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
#include "VaRGB.h"
#include "includes/VaRGBPlatform.h"



namespace vargb {



VaRGB::VaRGB(VaRGB_SetColor_Callback set_color_with_cb, VaRGB_Schedule_Completed sched_comp_cb) :
		set_color_cb(set_color_with_cb),
		set_color_forsched_cb(NULL),
		sched_completed_cb(sched_comp_cb),
		current_schedule(NULL),
		tick_count(0)
{

}

VaRGB::VaRGB(VaRGB_SetColorForSchedule_Callback set_color_for_sched_with_cb, VaRGB_Schedule_Completed sched_comp_cb) :
		set_color_cb(NULL),
		set_color_forsched_cb(set_color_for_sched_with_cb),
		sched_completed_cb(sched_comp_cb),
		current_schedule(NULL),
		tick_count(0)
{

}

void VaRGB::resetTicks()
{
	tick_count = 0;
	// TODO: FIXME ? reset schedule? don't think so (happens in setSchedule)
}

void VaRGB::setSchedule(Schedule* sched)
{
	current_schedule = sched;
	current_schedule->setDriver(this);

	if (current_schedule)
	{
		current_schedule->setTick(tick_count);

	}

}

void VaRGB::scheduleComplete(Schedule* sched)
{
	if (sched_completed_cb)
	{
		sched_completed_cb(sched);
	} else {
		// no schedule-completed callback... will be keeping this
		// sched, I guess, so we auto-reset
		resetTicks();
		setSchedule(current_schedule);

	}
}

void VaRGB::setColor(Schedule* for_sched, ColorSettings * setTo)
{
	if (set_color_cb)
	{
		set_color_cb(setTo);
	} else if (set_color_forsched_cb)
	{
		set_color_forsched_cb(for_sched, setTo);
	}

}

void VaRGB::tick(uint8_t amount)
{
	current_schedule->tick(amount);
	tick_count += amount;
}

void VaRGB::tickAndDelay(uint8_t num)
{
	tick(num);
	vargb::delayMs(num * tickDelayTimeMs());

}

} /* end namespace vargb */
