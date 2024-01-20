#include "robot.h"

namespace robot {

	void walk_to_wall() {
	start:
		if (isAtWall()) goto end;
		step();
		goto start;
	end:;
	}
	void turn_around() {
		turnCw();
		turnCw();
	}

	void robot_main() {
	a0: walk_to_wall();
		turn_around();
		goto a0;
	}
}
