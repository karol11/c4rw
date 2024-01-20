#include "robot.h"

// 

namespace robot {

	void robot_main() {
		step();
		turnCw();
		step();
		step();
		turnCw();
		turnCw();
		turnCw();
		step();
		step();
		turnCw();
		step();
		step();
		step();
		step();
		step();
		step();
		step();   // here robot hits the wall
		turnCw(); // this commands won't execute
		step();
	}
}
