#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <vector>
#include <array>

#include "object.h"

class RigidBody : public Component {
public:
    RigidBody();
    RigidBody(float mass, bool useGravity, bool isPinned);

    void addForce(glm::vec3 force);
    void clearForce();

    float mass;
    bool useGravity;
    bool isPinned;

    glm::vec3 force;
    glm::vec3 velocity;
    glm::vec3 acceleration;
};

#endif