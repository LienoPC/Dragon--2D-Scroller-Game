#include "../glm-master/glm/glm.hpp"


class Window {

public: 
	glm::vec2 Position;
	double offsetInterval;
	int identificator;

	Window();

	Window(glm::vec2 Position, double offsetInterval, int identificator);


};
