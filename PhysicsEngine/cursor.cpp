#include "cursor.h"

Cursor::Cursor() {
	// Create shader objects
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Set VAO and VBO for use
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Define pointers to attributes
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind buffer and array
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cursor::draw() {
	float aspect = static_cast<float>(1200) / 900;

	std::vector<glm::vec3> plus = {
		{-0.05f / aspect,  0.0f, 0.0f},
		{ 0.05f / aspect,  0.0f, 0.0f},
		{ 0.0f, -0.05f, 0.0f},
		{ 0.0f,  0.05f, 0.0f}
	};

	// Load cursor data into buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, plus.size() * sizeof(glm::vec3), plus.data());

	// Select VAO to use for interpertation
	glBindVertexArray(VAO);

	glLineWidth(3.0f);
	glDrawArrays(GL_LINES, 0, 4);

	glBindVertexArray(0);
}