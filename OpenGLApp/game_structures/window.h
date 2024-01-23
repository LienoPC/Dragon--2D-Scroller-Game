#ifndef WINDOW_H
#define WINDOW_H
#include "../glm-master/glm/glm.hpp"


class ThrowWindow {

public:
	glm::vec2 Position;
	double offsetInterval;
	int identificator;

	ThrowWindow();

	ThrowWindow(glm::vec2 Position, double offsetInterval, int identificator);


};

#endif