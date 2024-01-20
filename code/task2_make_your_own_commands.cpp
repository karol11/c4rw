#include "robot.h"

namespace robot {

	void right() {
		turnCw();
	}

	void left() {
		hide();
		right();
		right();
		right();
		show();
	}

	void turn_around() {
		turnCw();
		turnCw();
	}

	void step_back() {
		hide();
		turn_around();
		step();
		turn_around();
		show();
	}

	void robot_main() {
		step_back();
		left();
		step_back();
		step_back();
		step_back();
		step_back();
		step_back();
	}

}
