#ifndef FLAT_RENDERER_H
#define FLAT_RENDERER_H

#include "../resource_manager/resource_manager.h"
#include "../glad/include/glad/glad.h"

#include "../shaders_textures/shader.h"

class FlatRenderer {

public:
	FlatRenderer(Shader shader);
	
	void DrawFlatRectangle(glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);

private:
	Shader shader;


};


#endif // !FLAT_RENDERER_H


