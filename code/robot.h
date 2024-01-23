#ifndef ROBOT_H_
#define ROBOT_H_

#include <initializer_list>

namespace robot {

	void hide();
	void show();

	void step();
	void turnCw();
	bool isAtWall();
	bool isOnCross();

	bool isOnMark();
	void setMark();
	void clearMark();

	bool isOnArrow();
	bool isSameArrowDirection();
	void setArrow();
	void clearArrow();

	bool isOnChar();
	void setChar(char c);
	char getChar();
	void clearChar();

	void saveField(const char* file_name);
	void loadField(const char* file_name);
	void makeField(int dir, std::initializer_list<char*> field);
} // namespace robot

#endif // ROBOT_H_
