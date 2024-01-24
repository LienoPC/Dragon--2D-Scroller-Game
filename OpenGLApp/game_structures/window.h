#ifndef WINDOW_H
#define WINDOW_H
#include "../glm-master/glm/glm.hpp"

enum DirectionStart{UP, LEFT, RIGHT, DOWN};

class ThrowWindow {

public:
	glm::vec2 Position;
	glm::vec2 offsetInterval;
	int identificator;
	DirectionStart directionStart;

	ThrowWindow();

	ThrowWindow(glm::vec2 Position, glm::vec2 offsetInterval, int identificator, DirectionStart directionStart);


};

#endif