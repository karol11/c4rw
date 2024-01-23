#include "robot.h"

namespace robot {

	void walkToWall() {
	start:
		if (isAtWall()) goto end;
		step();
		goto start;
	end:;
	}
	void turnAround() {
		turnCw();
		turnCw();
	}

	void robotMain() {
	a0: walkToWall();
		turnAround();
		goto a0;
	}
}
