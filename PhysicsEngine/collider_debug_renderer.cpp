#include "collider_debug_renderer.h"

ColliderDebugRenderer::ColliderDebugRenderer() : VAO(0), VBO(0) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24* sizeof(unsigned int), nullptr, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glEnableVertexAttribArray(0);

	// Unbind buffer and array
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ColliderDebugRenderer::draw(BoundingBox boundingBox) {
    glm::vec3 worldMin = boundingBox.min;
    glm::vec3 worldMax = boundingBox.max;

    glm::vec3 vertices[] = {
        { worldMin.x, worldMin.y, worldMin.z },
        { worldMax.x, worldMin.y, worldMin.z },
        { worldMax.x, worldMax.y, worldMin.z },
        { worldMin.x, worldMax.y, worldMin.z },

        { worldMin.x, worldMin.y, worldMax.z },
        { worldMax.x, worldMin.y, worldMax.z },
        { worldMax.x, worldMax.y, worldMax.z },
        { worldMin.x, worldMax.y, worldMax.z }
    };

    unsigned int indices[] = {
        0, 1,  1, 2,  2, 3,  3, 0, // front
        4, 5,  5, 6,  6, 7,  7, 4, // back
        0, 4,  1, 5,  2, 6,  3, 7  // sides
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindBuffer(GL_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(indices), indices);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, nullptr);
}