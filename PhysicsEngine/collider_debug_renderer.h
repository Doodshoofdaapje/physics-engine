#ifndef COLLIDER_DEBUG_RENDERER_H
#define COLLIDER_DEBUG_RENDERER_H

#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <vector>

#include "collider_aabb.h"

class ColliderDebugRenderer {

public:
	ColliderDebugRenderer();
	void draw(BoundingBox boundingBox);

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

#endif


