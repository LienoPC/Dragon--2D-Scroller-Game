#ifndef PARTICLE_H
#define PARTICLE_H

#include "../../glm-master/glm/glm.hpp"

class Particle {

public:
	glm::vec2 Position, Velocity, Size;
	glm::vec4 Color;
	float Life;
	float Rotation;

	Particle() :
		Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f), Size(1.0f), Rotation(0.0f) {

	}
};


#endif