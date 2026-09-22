#include "transform.h"


Transform::Transform() {
	this->position = { 0,0,0 };
	this->rotation = { 0,0,0 };
	this->scale = { 1, 1, 1 };
}

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

void Transform::translate(glm::vec3 position) {
	this->position += position;
}

void Transform::setPosition(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

glm::mat4 Transform::getModel() const {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around x
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around y
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around z
	model = glm::scale(model, scale);
	return model;
}