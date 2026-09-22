#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/geometric.hpp>

#include <string>
#include <vector>
#include <array>

#include "solver.h"
#include "object.h"
#include "gravity.h"
#include "collider_aabb.h"

#pragma once
class PhysicsEngine {

public:
    bool enabled;

    PhysicsEngine();

    // Disable copying
    PhysicsEngine(const PhysicsEngine&) = delete;
    PhysicsEngine& operator=(const PhysicsEngine&) = delete;

    // Register object to physics checks
    void registerObject(Object* object);
    void registerForce(std::unique_ptr<Force> force);
    std::vector<Force*> getForces();

    // Adds force vectors to objects
    void applyForces();
    // Sets forces vectors of all objects to 0
    void clearForces();

    void toggleSimulation();
    void simulationStep(float dt);

private:
    // Physics objects
    std::vector<std::unique_ptr<Force>> forces;
    std::vector<Object*> objects;

    // Numeric solver
    std::unique_ptr<Solver> solver;

    // Collision checks
    void resolveCollisions();
};

#endif