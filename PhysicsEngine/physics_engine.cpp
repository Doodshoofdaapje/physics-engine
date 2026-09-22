#include "physics_engine.h"
#include "rigid_body.h"
#include "solver_runge_kutta_4.h"
#include "solver_euler.h"
#include "gravity.h"
#include "spring.h"

PhysicsEngine::PhysicsEngine() {
	this->solver = std::make_unique<SolverEuler>(objects, forces);
	this->enabled = false;
}

void PhysicsEngine::registerObject(Object* object) {
	if (!object->hasComponent<RigidBody>()) {
		return;
	}

	RigidBody* rb = object->getComponent<RigidBody>();
	if (rb->useGravity) {
		forces.push_back(std::make_unique<Gravity>(object, 9.81f));
	}

	objects.push_back(object);
}

void PhysicsEngine::registerForce(std::unique_ptr<Force> force) {
	forces.push_back(std::move(force));
}

std::vector<Force*> PhysicsEngine::getForces() {
	std::vector<Force*> results;
	for (auto& force : forces) {
		results.push_back(force.get());
	}
	return results;
}

void PhysicsEngine::clearForces() {
	for (Object* object : objects) {
		RigidBody* rb = object->getComponent<RigidBody>();
		rb->clearForce();
	}
}

void PhysicsEngine::applyForces() {
	for (auto& force : forces) {
		force->apply();
	}
}

void PhysicsEngine::simulationStep(float dt) {
	if (!enabled) {
		return;
	}
	clearForces();
	applyForces();

	checkCollisions();

	solver->simulationStep(dt);
}

void PhysicsEngine::toggleSimulation() {
	this->enabled = !this->enabled;
}


void PhysicsEngine::checkCollisions() {
	for (size_t i = 0; i < objects.size(); ++i) {
		Object* obj1 = objects[i];
		if (!obj1->hasComponent<ColliderAABB>())
			continue;

		for (size_t j = i + 1; j < objects.size(); ++j) {
			Object* obj2 = objects[j];

			if (!obj2->hasComponent<ColliderAABB>())
				continue;

			if (collides(obj1, obj2))
				handleCollision(obj1, obj2);
		}
	}
}

bool PhysicsEngine::collides(Object* obj1, Object* obj2) {
	Transform* tr1 = obj1->getComponent<Transform>();
	Transform* tr2 = obj2->getComponent<Transform>();
	ColliderAABB* col1 = obj1->getComponent<ColliderAABB>();
	ColliderAABB* col2 = obj2->getComponent<ColliderAABB>();
	
	BoundingBox bb1 = col1->getWorldBounds(tr1);
	BoundingBox bb2 = col2->getWorldBounds(tr2);

	return
		bb1.min.x <= bb2.max.x && bb1.max.x >= bb2.min.x &&
		bb1.min.y <= bb2.max.y && bb1.max.y >= bb2.min.y &&
		bb1.min.z <= bb2.max.z && bb1.max.z >= bb2.min.z;
}

void PhysicsEngine::handleCollision(Object* obj1, Object* obj2) {
	Transform* tr1 = obj1->getComponent<Transform>();
	Transform* tr2 = obj2->getComponent<Transform>();
	ColliderAABB* col1 = obj1->getComponent<ColliderAABB>();
	ColliderAABB* col2 = obj2->getComponent<ColliderAABB>();
	RigidBody* rb1 = obj1->getComponent<RigidBody>();
	RigidBody* rb2 = obj2->getComponent<RigidBody>();

	BoundingBox bb1 = col1->getWorldBounds(tr1);
	BoundingBox bb2 = col2->getWorldBounds(tr2);

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
	
	float totalInverseMass =
		1.0f / rb1->mass +
		1.0f / rb2->mass;

	// Move objects out of eachother
	if (!rb1->isPinned)
		tr1->position += collisionNormal * penetration * (1.0f / rb1->mass) / totalInverseMass;
	if (!rb2->isPinned)
		tr2->position -= collisionNormal * penetration * (1.0f / rb2->mass) / totalInverseMass;

	const float elasticity = 0.95f;
	float relativeVelocity = glm::dot(rb1->velocity - rb2->velocity, collisionNormal);

	if (relativeVelocity > 0.0f)
		return;

	float impulseMagnitude = -(1.0f + elasticity) * relativeVelocity;
	impulseMagnitude /= totalInverseMass;

	glm::vec3 impulse = impulseMagnitude * collisionNormal;

	rb1->velocity += impulse / rb1->mass;
	rb2->velocity -= impulse / rb2->mass;
}