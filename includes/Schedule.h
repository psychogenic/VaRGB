/*

 Schedule.h -- part of the VaRGB library.
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
 A schedule is basically a manager for a list of RGB transition curves.

 You set it up by adding each curve in the order in which you want them
 to run, and then hand the schedule to the VaRGB driver.

 Encapsulating transitions in schedules allows you to easily swap out
 one set of curves for another.

 If your code is dynamically assigning schedules or otherwise doing
 anything fancy, you can attribute an ID to each schedule and then,
 when a given schedule has run its course (or at any point, really)
 you can query the schedule for its ID and replace it in the driver
 with whichever schedule is appropriate at that time.

 Schedules maintain a list of pointers to Curves.  This list is
 dynamically expanded as required by the number of transitions you
 add.

*/

#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include "VaRGBConfig.h"
#include "VaRGBPlatform.h"
#include "Curve.h"

#define VaRGB_SCHEDULE_CURVELIST_EXPAND_BYAMOUNT	5

namespace vargb {

typedef uint8_t ScheduleID;
class VaRGB; // forward declaration

class Schedule {
public:
	/*
	 * Schedule constructor
	 * You may optionally pass an ID (integer < 256), or let it
	 * be specified automatically.
	 *
	 * NOTE: It's probably safest to avoid mixing styles--either
	 * set all the schedule IDs manually or let them all be assigned
	 * automatically.
	 */
	Schedule(ScheduleID sched_id=0);

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
	~Schedule();
#endif

	/*
	 * addTransition
	 * Pass a pointer to an RGB transition curve to this method
	 * to add the curve to the schedule.
	 *
	 * NOTE: This is a pointer to a Curve object, which can be
	 * dynamically allocated
	 * 	... = new SomeCurve(...);
	 * or simply passed as the address of some object
	 * 	&someCurveObject
	 * Either way, that pointer MUST be valid during the lifetime
	 * of the schedule, or "you're gonna have a bad time".
	 */
	bool addTransition(vargb::Curve::Curve * curv);

	void setTick(VaRGBTimeValue tick_count);

	void tick(uint8_t num=1);

	void setDriver(VaRGB* drv) { driver = drv;}

	/*
	 * id
	 * Returns the ID for this schedule.
	 */
	ScheduleID id() { return sched_id;}
private:

	static ScheduleID schedule_counter;

	ScheduleID sched_id;
	vargb::Curve::Curve ** transition_ptr_list;
	uint8_t transition_index; // current index
	uint8_t transition_num; // number in list
	uint8_t transition_max; // max space in list
	uint16_t total_schedule_ticks;

	VaRGB * driver;


	bool expandTransitionList(uint8_t byamount=VaRGB_SCHEDULE_CURVELIST_EXPAND_BYAMOUNT);

	void sendCurTransitionSettings();



};

} /* namespace vargb */
#endif /* SCHEDULE_H_ */
