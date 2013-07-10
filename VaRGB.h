/*

 VaRGB.h -- VaRGB driver, part of the VaRGB library.
 Copyright (C) 2013 Pat Deegan.

 http://www.flyingcarsandstuff.com/projects/vargb/

 Created on: 2013-03-03

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 See file LICENSE.txt for further informations on licensing terms.

 *****************************  OVERVIEW  *****************************

 After performing object instantiation and setup, the VaRGB driver--defined
 here--is the object user code will spend most time interacting with while
 using VaRGB.

 In essence, the process is:

 * create callback(s), for setting the color and optionally to be notified
   when a schedule has completed all its curves:

   void myColorSettingCallback(vargb::ColorSettings * set_to)
   {
     // ... do something with the ColorSettings--this is
     // hardware dependent, and up to you
     // The set_to pointer has red, green and blue attributes which
     // are set to the value each color component should take.
   }

 * create the driver, passing it the function(s) to use as callback(s):
 vargb::VaRGB myDriver(myColorSettingCallback);

 * create curves and schedules, adding the transition curves to the schedules
   as required.  See the sample code for lots of this...


 * Set a schedule in the driver:

    myDriver.setSchedule(a_pointer_to_schedule);

 * Finally, in your main loop, let time pass and notify VaRGB of the passage of time

   void loop() {

      // tell the VaRGB driver that time has passed, it will figure out
      // if it needs to adjust the colors and use the callback as necessary
      myDriver.tick();

      // do whatever else you may need to do


      delay(appropriate_number_of_milliseconds); // only tick every VaRGB::tickDelayTimeMs() millis

   }

 Just how you figure out how much to delay between tick()s is described more fully in the examples,
 see Basics.ino and the other samples for details.


 You may also construct the driver with a "schedule-completed" callback, which will be called when
 the current schedule has run its course.  Within, you may reset the schedule, or change it to
 another or do whatever you like.

 For the "set-color" callback, you can use a second version of the function--one which takes
 the current schedule as a parameter--if you need to do anything fancy when updating colors that
 depends on which schedule is running.

*/
#ifndef VARGB_H_
#define VARGB_H_

#define VARGB_VERSION		1
#define VARGB_SUBVERSION	0

#include "includes/VaRGBConfig.h"
#include "includes/VaRGBPlatform.h"
#include "includes/Schedule.h"

namespace vargb {


/*
 * Signature for the "basic" set-color callback.  The function must have the form:
 *
 * void myfunctionname(vargb::ColorSettings * set_colors_to);
 */
typedef void (*VaRGB_SetColor_Callback)(ColorSettings * set_colors_to);

/*
 * Signature for the "extended" set-color callback.  The function must have the form:
 *
 * void setcolorfuncname(vargb::Schedule* the_schedule, vargb::ColorSettings * set_colors_to);
 */
typedef void (*VaRGB_SetColorForSchedule_Callback)(Schedule* sched, ColorSettings * set_colors_to);

/*
 * Signature for the schedule-completed callback.  This function must have the form:
 *
 * void myschedulecompletedcb(vargb::Schedule* the_schedule);
 *
 */
typedef void (*VaRGB_Schedule_Completed) (Schedule* sched);


class VaRGB {
public:
	/*
	 * VaRGB Constructors.  There are two versions, each takes a color-setting callback as the first
	 * parameter and an optional schedule-completed callback as the second.
	 *
	 * All you need to do is create callbacks with the appropriate signature, as described above, and
	 * create your object:
	 *
	 * 	vargb::VaRGB v_driver(set_color_cb_name [, schedule_complete_cb_name]);
	 *
	 */
	VaRGB(VaRGB_SetColor_Callback set_color_with_cb, VaRGB_Schedule_Completed sched_comp_cb=NULL);
	VaRGB(VaRGB_SetColorForSchedule_Callback set_color_for_sched_with_cb, VaRGB_Schedule_Completed sched_comp_cb=NULL);


	/*
	 * tickDelayTimeMs
	 * There should be a certain delay between your calls to tick().  The number of milliseconds expected between
	 * such calls is returned by VaRGB::tickDelayTimeMs()
	 */
#if VaRGB_DELAY_BETWEEN_UPDATES_MS > 255
	inline static uint16_t tickDelayTimeMs() { return VaRGB_DELAY_BETWEEN_UPDATES_MS;}
#else
	inline static uint8_t tickDelayTimeMs() { return VaRGB_DELAY_BETWEEN_UPDATES_MS; }
#endif

	/*
	 * Though they are set during construction, you can change the set-color and schedule-completed callbacks using the
	 * aptly named setColorSetCallback and setScheduleCompletedCallback methods.
	 */
	inline void setColorSetCallback(VaRGB_SetColor_Callback cb) {set_color_cb = cb; set_color_forsched_cb = NULL;}
	inline void setColorSetCallback(VaRGB_SetColorForSchedule_Callback cb) {set_color_forsched_cb = cb; set_color_cb = NULL;}

	inline void setScheduleCompletedCallback(VaRGB_Schedule_Completed cb) { sched_completed_cb = cb;}


	/*
	 * schedule
	 * Return a pointer to the currently assigned schedule.
	 */
	Schedule* schedule() { return current_schedule; }
	/*
	 * setSchedule
	 * Set the current schedule by passing a pointer to it to this method.
	 */
	void setSchedule(Schedule* sched) ;

	/*
	 * resetTicks
	 * The VaRGB driver keeps a count of ticks.  Calling resetTicks sets this back to 0
	 */
	void resetTicks();
	/*
	 * tickCount
	 * Returns the current tick count.
	 */
	VaRGBTimeValue tickCount() { return tick_count;}



	/*
	 * tick
	 * Inform the schedule and its currently running curve of the passage of time, by tick()ing the clock.
	 *
	 * Do this every VaRGB::tickDelayTimeMs() milliseconds, as described above and in the samples.
	 */
	void tick(uint8_t num=1);

	/*
	 * tickAndDelay
	 * If your code isn't spending much time doing anything other than driving the RGB LEDs, then you
	 * can use this convenience function to tick() and delay tickDelayTimeMs() millis for you.
	 */
	void tickAndDelay(uint8_t num=1);


	// used internally: setColor/scheduleComplete (called by Schedule on driver to notify callbacks)
	void setColor(Schedule* for_sched, ColorSettings * setTo);

	void scheduleComplete(Schedule* sched);


private:
	VaRGB_SetColor_Callback set_color_cb;
	VaRGB_SetColorForSchedule_Callback set_color_forsched_cb;
	VaRGB_Schedule_Completed sched_completed_cb;
	Schedule * current_schedule;
	VaRGBTimeValue tick_count;



};

} /* end namespace vargb */

#endif /* VARGB_H_ */
