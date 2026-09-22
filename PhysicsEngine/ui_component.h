#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <vector>

class UiComponent {

public:    
	UiComponent() : VAO(0), VBO(0) {}
	virtual ~UiComponent() = default;
	virtual void draw() = 0;

protected:
	unsigned int VAO;
	unsigned int VBO;
};

#endif
