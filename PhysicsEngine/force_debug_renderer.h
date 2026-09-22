#ifndef FORCE_DEBUG_RENDERER_H
#define FORCE_DEBUG_RENDERER_H

#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <vector>

class ForceDebugRenderer {

private:
	unsigned int VAO;
	unsigned int VBO;

public:
	ForceDebugRenderer();
	void draw(std::vector<glm::vec3> debugVector);
};

#endif


