#include "PhysicsEngine.h"

void PhysicsEngine::Update()
{
	// Broad phase follows a producer/consumer pattern (sort/sweep).
	// First, we "sort" all endpoints and produce a set of instructions.
	// Then, we "sweep" and consume this set of instructions to generate a set 
	// of overlapping intervals.
	CheckBroadPhase();

	// Check for collisions more precisely, between colliders for detected
	// overlaps.
	CheckNarrowPhase();

	// Do something with the detected collisions.
	HandleCollisions();
}

void PhysicsEngine::AddPhysicsObject(GLuint gameObjectId, Collider *collider,
	std::function<void(const PhysicsObject &)> onCollision)
{
	std::unique_ptr<PhysicsObject> physObj{ std::make_unique<PhysicsObject>(
		gameObjectId, collider, onCollision) };
	m_physObjects.push_back(std::move(physObj));
}

void PhysicsEngine::CheckBroadPhase()
{
}

void PhysicsEngine::CheckNarrowPhase()
{
}

void PhysicsEngine::HandleCollisions()
{
}

void PhysicsEngine::UpdateEndpoints()
{
}

void PhysicsEngine::UpdateIntervals(std::vector<Endpoint> &endpoints, 
	std::vector<int> &lookupTable)
{
}

void PhysicsEngine::GenerateOverlapsSet()
{
}

bool PhysicsEngine::IsOverlapping(int endpointIndex1, int endpointIndex2, 
	const std::vector<int> &lookupTable, const std::vector<Endpoint> &endpoints)
{
	return false;
}