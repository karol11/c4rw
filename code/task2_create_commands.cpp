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

	void turnAround() {
		turnCw();
		turnCw();
	}

	void stepBack() {
		turnAround();
		step();
		turnAround();
	}

	void robotMain() {
		stepBack();
		left();
		stepBack();
		stepBack();
		stepBack();
		stepBack();
		stepBack();
	}

}
