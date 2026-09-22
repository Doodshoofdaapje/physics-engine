#include "collider_aabb.h"

ColliderAABB::ColliderAABB(const Mesh& mesh) {
    this->bounds = { mesh.getMinBounds(), mesh.getMaxBounds() };
}

void ColliderAABB::setBoundingBox(glm::vec3 minBounds, glm::vec3 maxBounds) {
    this->bounds = { minBounds, maxBounds };
}

const BoundingBox& ColliderAABB::getLocalBounds() {
	return this->bounds;
}

const BoundingBox ColliderAABB::getWorldBounds(const Transform* transform) const
{
    glm::vec3 localMin = bounds.min;
    glm::vec3 localMax = bounds.max;

    glm::vec3 corners[] = {
        { localMin.x, localMin.y, localMin.z },
        { localMax.x, localMin.y, localMin.z },
        { localMax.x, localMax.y, localMin.z },
        { localMin.x, localMax.y, localMin.z },
        { localMin.x, localMin.y, localMax.z },
        { localMax.x, localMin.y, localMax.z },
        { localMax.x, localMax.y, localMax.z },
        { localMin.x, localMax.y, localMax.z }
    };

    glm::vec3 worldMin(std::numeric_limits<float>::max());
    glm::vec3 worldMax(std::numeric_limits<float>::lowest());

    for (const auto& corner : corners) {
        glm::vec3 world =
            glm::vec3(transform->getModel() * glm::vec4(corner, 1.0f));

        worldMin = glm::min(worldMin, world);
        worldMax = glm::max(worldMax, world);
    }

    return { worldMin, worldMax };
}