/*

 Curve.h -- part of the VaRGB library.
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

 This is the base class for our curves.  "Transition curves" describe how
 R-G-B values will be changing with time.

 Some upper-layer component, e.g. the schedule which is holding the curve,
 will inform the curve of the passage of time, by using the curve's tick()
 method.

 Should the curve determine that an update to the Red, Green and/or Blue
 values is necessary, it simply sets its (protected member) settings_req_update
 to TRUE.  The caller (the schedule, above) is responsible for checking whether
 an update is needed (using settingsNeedUpdate()), adjusting the hardware according
 to the curve's currentSettings() and informing the curve that the outside world
 now reflects the values specified (using settingsUpdated()).

 Curves are also responsible for terminating, by setting their (protected member)
 curve_completed flag to TRUE when appropriate.  It is the user's (schedule, etc.)
 job to periodically check if the_curve.completed() and, if so, act accordingly.


*/

#ifndef BASECURVE_H_
#define BASECURVE_H_

#include "VaRGBConfig.h"
#include "VaRGBPlatform.h"

#define VaRGB_NUM_COLORS	3


#define vargb_red_idx  		0
#define vargb_green_idx 	1
#define vargb_blue_idx 		2

namespace vargb {

/*
 * IlluminationTarget
 * A structure that holds data about target R-G-B settings and
 * the number of ticks to spend "getting there".
 */
typedef struct IlluminationTargetStruct {

	IlluminationTargetStruct();

	IlluminationTargetStruct(VaRGBColorValue red, VaRGBColorValue green,
			VaRGBColorValue blue, VaRGBTimeValue trans_time_seconds = 1);

	VaRGBColorValue values[VaRGB_NUM_COLORS];
	uint16_t transition_ticks;

} IlluminationTarget;


/* IlluminationSettings
 * Represents R-G-B settings, here we just re-use the
 * IlluminationTarget struct.
 */
typedef IlluminationTarget IlluminationSettings;




typedef struct ColorSettings {
	VaRGBColorValue red;
	VaRGBColorValue green;
	VaRGBColorValue blue;
} ColorSettings;



#ifdef VaRGB_ENABLE_CURVE_LOGICAL
// if combos are enabled, we need to be able to
// override a few extra methods.
#define VARGB_CURVE_VIRTMETHOD_PREFIX				virtual
#define VARGB_CURVE_VIRTORINLINE_METHOD_PREFIX		virtual
#else
// combos aren't enabled, so lets save on some
// processing
#define	 VARGB_CURVE_VIRTMETHOD_PREFIX
#define VARGB_CURVE_VIRTORINLINE_METHOD_PREFIX		inline
#endif

namespace Curve {

/*
 *  Using curves...
 * while (! my_curve.completed())
 * {
 * 		my_curve.tick();
 * 		if (my_curve.settingsNeedUpdate())
 * 		{
 * 			IlluminationSettings * illum = my_curve.currentSettings();
 * 			// update settings
 * 			// ...
 * 			// then
 * 			//
 * 			my_curve.settingsUpdated();
 * 		}
 *
 * 		delay(TIME_FOR_ONE_TICK);
 * 	}
 *
 */

class Curve {
public:
	/*
	 * createTarget class method
	 * A nice way to create an IlluminationTarget object without bothering with the details
	 * of the structure.
	 */
	static IlluminationTarget createTarget(VaRGBColorValue red, VaRGBColorValue green,
					VaRGBColorValue blue, VaRGBTimeValue trans_time_seconds);

	/*
	 * Curve constructor
	 * Pass it the target Red, Green and Blue settings, and the transition time (in seconds)
	 */
	Curve(VaRGBColorValue target_red, VaRGBColorValue target_green,
			VaRGBColorValue target_blue, VaRGBTimeValue trans_time_seconds);

#ifdef VaRGB_CLASS_DESTRUCTORS_ENABLE
	virtual ~Curve();
#endif

	/*
	 * target()
	 *
	 * Returns a pointer to the IlluminationTarget aimed for by this curve.
	 */
	inline IlluminationTarget * target() { return &curve_target;}

	/*
	 * setTarget()
	 * Set the IlluminationTarget for the curve.
	 */
	void setTarget(IlluminationTarget target) { curve_target = target;}


	/*
	 * completed()
	 *
	 * Used by Curve user.
	 *
	 * Returns boolean TRUE value if this curve has completed its run (set by curves using
	 * the curve_completed protected member boolean flag).
	 */
	VARGB_CURVE_VIRTORINLINE_METHOD_PREFIX bool completed() { return curve_completed; }


	/*
	 * settingsNeedUpdate()
	 *
	 * Used by Curve user.
	 *
	 * Returns boolean TRUE value if the curve deems it is necessary to perform an
	 * update to the current R,G and/or B settings (set by curves using the
	 * settings_req_update protected member boolean flag).
	 *
	 */
	inline bool settingsNeedUpdate() { return settings_req_update;}

	/*
	 * settingsUpdated()
	 * Called by upper-layer to notify curve that its current settings have been reflected by
	 * the hardware/outside world.
	 */
	VARGB_CURVE_VIRTORINLINE_METHOD_PREFIX void settingsUpdated() { settings_req_update = false; }

	/*
	 * start()
	 *
	 * Called by curve user to notify the curve that we are going to begin its transition,
	 * i.e. reset the current tick count to 0 and, optionally, specify the starting conditions
	 * (current illumination settings).
	 */
	VARGB_CURVE_VIRTMETHOD_PREFIX void start(IlluminationSettings* initial_settings=NULL);

	/*
	 * setTick()
	 * Similar to start, but used to set the tick count to a non-zero value (move forward in time).
	 */
	virtual void setTick(VaRGBTimeValue setTo, IlluminationSettings* initial_settings=NULL) = 0;

	/*
	 * tick()
	 * Let the curve know that some time has passed.  The curve's job here is to see whether it needs
	 * to change the current R-G-B settings and/or if it has completed its run and notify the caller
	 * of each condition using the settings_req_update/curve_completed flags described above.
	 */
	virtual void tick(uint8_t num=1) = 0;


	/*
	 * currentSettings()
	 *
	 * Returns a pointer to what the curve believes should be the current illumination settings.
	 */
	inline IlluminationSettings * currentSettings() { return &current_settings;}

	/*
	 * resetCurrentSettings()
	 *
	 * Set the current settings according to the pointer passed or, if NULL, to all-zeros.
	 */
	void resetCurrentSettings(IlluminationSettings * setTo=NULL);


	/*
	 * Reset tick count and all relevant flags.  Called automatically by start()
	 */
	VARGB_CURVE_VIRTMETHOD_PREFIX void reset();

protected:


	bool settings_req_update;
	bool curve_completed;
	VaRGBTimeValue tick_count;
	IlluminationTarget curve_target;
	IlluminationSettings current_settings;


};

} /* namespace Curve */
} /* namespace vargb */
#endif /* BASECURVE_H_ */
