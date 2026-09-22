#ifndef COLLIDER_AABB_H
#define COLLIDER_AABB_H

#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <vector>
#include <array>

#include "object.h"
#include "mesh_renderer.h"
#include "rigid_body.h" 

struct BoundingBox {
    glm::vec3 min;
    glm::vec3 max;
};

class ColliderAABB : public Component {

public:
    ColliderAABB(const Mesh& mesh);

    void setBoundingBox(glm::vec3 minBounds, glm::vec3 maxBounds);
    
    const BoundingBox& getLocalBounds();
    const BoundingBox getWorldBounds() const;

    bool collidesWith(ColliderAABB* other) const;
    void resolveCollision(ColliderAABB* other);

private:
    BoundingBox bounds;

};

#endif