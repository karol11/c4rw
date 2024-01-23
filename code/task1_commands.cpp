#include "robot.h"

// 

namespace robot {

	void draw() {
		setMark();
		step();
	}

	void robotMain() {
		draw();
		draw();
		draw();
	}
}
