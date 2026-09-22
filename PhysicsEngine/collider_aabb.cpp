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

const BoundingBox ColliderAABB::getWorldBounds() const
{
    Transform* transform = getObject()->getComponent<Transform>();
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

bool ColliderAABB::collidesWith(ColliderAABB* other) const {
    BoundingBox thisBB = getWorldBounds();
    BoundingBox otherBB = other->getWorldBounds();

    return
        thisBB.min.x <= otherBB.max.x && thisBB.max.x >= otherBB.min.x &&
        thisBB.min.y <= otherBB.max.y && thisBB.max.y >= otherBB.min.y &&
        thisBB.min.z <= otherBB.max.z && thisBB.max.z >= otherBB.min.z;
}


void ColliderAABB::resolveCollision(ColliderAABB* other) {
	Transform* tr1 = getObject()->getComponent<Transform>();
	Transform* tr2 = other->getObject()->getComponent<Transform>();
	ColliderAABB* col1 = getObject()->getComponent<ColliderAABB>();
	ColliderAABB* col2 = other->getObject()->getComponent<ColliderAABB>();
	RigidBody* rb1 = getObject()->getComponent<RigidBody>();
	RigidBody* rb2 = other->getObject()->getComponent<RigidBody>();

	BoundingBox bb1 = col1->getWorldBounds();
	BoundingBox bb2 = col2->getWorldBounds();

	// Determine collision side and penetration depth
	float overlapX = std::min(bb1.max.x, bb2.max.x) - std::max(bb1.min.x, bb2.min.x);
	float overlapY = std::min(bb1.max.y, bb2.max.y) - std::max(bb1.min.y, bb2.min.y);
	float overlapZ = std::min(bb1.max.z, bb2.max.z) - std::max(bb1.min.z, bb2.min.z);

	float penetration = overlapX;
	glm::vec3 collisionNormal = glm::vec3(tr1->position.x < tr2->position.x ? -1.0f : 1.0f, 0.0f, 0.0f);

	if (overlapY < penetration) {
		penetration = overlapY;
		collisionNormal = glm::vec3(0.0f, tr1->position.y < tr2->position.y ? -1.0f : 1.0f, 0.0f);
	}

	if (overlapZ < penetration) {
		penetration = overlapZ;
		collisionNormal = glm::vec3(0.0f, 0.0f, tr1->position.z < tr2->position.z ? -1.0f : 1.0f);
	}

	// Fix collision

	float invMass1 = rb1->isPinned ? 0.0f : 1.0f / rb1->mass;
	float invMass2 = rb2->isPinned ? 0.0f : 1.0f / rb2->mass;

	float totalInverseMass = invMass1 + invMass2;

	if (totalInverseMass == 0.0f)
		return; // Both objects are pinned

	// Move objects out of eachother
	if (!rb1->isPinned)
		tr1->position += collisionNormal * penetration * invMass1 / totalInverseMass;
	if (!rb2->isPinned)
		tr2->position -= collisionNormal * penetration * invMass2 / totalInverseMass;

	const float elasticity = 0.95f;
	float relativeVelocity = glm::dot(rb1->velocity - rb2->velocity, collisionNormal);

	if (relativeVelocity > 0.0f)
		return;

	float impulseMagnitude = -(1.0f + elasticity) * relativeVelocity;
	impulseMagnitude /= totalInverseMass;

	glm::vec3 impulse = impulseMagnitude * collisionNormal;

	rb1->velocity += impulse * invMass1;
	rb2->velocity -= impulse * invMass2;
}