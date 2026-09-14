#pragma once
#include "../BaseInc.h"

RS_NS_START

// Stores all control inputs to a car
struct CarControls {
	// Driving control
	float throttle, steer;

	// Air orientation control
	float pitch, yaw, roll;

	// **The same three axes as the dodge sees them, when they are not the same numbers.**
	//
	// Rocket League hands its car two different versions of the stick. `ControllerInput` carries
	// the raw deadzoned axes, and that is what a dodge is decided and aimed from; the player's
	// air-control sensitivity is applied *downstream*, inside the component that turns an input
	// into torque. Measured, by moving the game's own sensitivity slider from 2.0 to 10: the gain
	// from the physical stick to `ControllerInput` does not move (0.955 to 0.957 on yaw, 0.951 to
	// 0.975 on pitch), while the rotation rate held at a quarter deflection goes from 0.843 to
	// 2.878 rad/s. So the slider scales the torque and not the input.
	//
	// With one set of fields a caller has to choose which of those two jobs to be right about. A
	// caller that scales before setting `pitch` gets the game's air control and a flip that fires
	// early -- at a sensitivity of 1.45, at 0.345 of stick travel where the game needs 0.50. A
	// caller that does not scale gets the flip right and loses the sensitivity entirely.
	//
	// So these carry the unscaled axes, and `useDodgeInput` says whether they were filled in.
	// Default false, which is exactly the old behaviour: everything reads `pitch/yaw/roll` as
	// before and no existing caller changes.
	float dodgePitch, dodgeYaw, dodgeRoll;
	bool useDodgeInput;

	// Boolean action inputs
	bool jump, boost, handbrake;

	// Maybe someday...
	// bool useItem;

	CarControls() {
		// Initialize everything as zero
		memset(this, 0, sizeof(CarControls));
	}

	// Makes all values range-valid (clamps from -1 to 1)
	void ClampFix() {
		throttle	= RS_CLAMP(throttle,	-1, 1);
		steer		= RS_CLAMP(steer,		-1, 1);
		pitch		= RS_CLAMP(pitch,		-1, 1);
		yaw			= RS_CLAMP(yaw,		-1, 1);
		roll		= RS_CLAMP(roll,		-1, 1);
		dodgePitch	= RS_CLAMP(dodgePitch,	-1, 1);
		dodgeYaw	= RS_CLAMP(dodgeYaw,	-1, 1);
		dodgeRoll	= RS_CLAMP(dodgeRoll,	-1, 1);
	}

	// What a dodge is decided and aimed from: the unscaled axes when a caller supplied them, and
	// the ordinary ones when it did not.
	float DodgePitch() const { return useDodgeInput ? dodgePitch : pitch; }
	float DodgeYaw()   const { return useDodgeInput ? dodgeYaw   : yaw;   }
	float DodgeRoll()  const { return useDodgeInput ? dodgeRoll  : roll;  }
};

#define CAR_CONTROLS_SERIALIZATION_FIELDS(name) \
name.throttle, name.steer, \
name.pitch, name.yaw, name.roll, \
name.boost, name.jump, name.handbrake

RS_NS_END