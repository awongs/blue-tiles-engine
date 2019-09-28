#include "PhysicsEngine.h"
#include "../debugbt/DebugLog.h"

#include <glm/glm.hpp>

#include <iostream>

void PhysicsEngine::Update()
{
	// Roughly check for potential collisions.
	DoBroadPhase();

	// Check for collisions more precisely, between colliders for detected
	// overlaps.
	DoNarrowPhase();

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

void PhysicsEngine::DoBroadPhase()
{
	// Broad phase follows a producer/consumer pattern (sort/sweep).
	// First, we "sort" all endpoints and produce a set of instructions.
	// Then, we "sweep" and consume this set of instructions to generate a set 
	// of overlapping intervals.

	// Update the endpoint values before sorting.
	UpdateEndpoints();

	// Sort phase: repopulate list of events to perform.
	m_events.clear();
	UpdateIntervals(m_endpointsX, m_lookupX);
	UpdateIntervals(m_endpointsY, m_lookupY);

	// Sweep phase: generate the set of overlaps from the events.
	GenerateOverlapsSet();
}

void PhysicsEngine::DoNarrowPhase()
{
	// TODO: implement sphere collider checks.
}

void PhysicsEngine::HandleCollisions()
{
	// TODO: maybe better way to do this?
	for (auto &col : m_collisions)
	{
		PhysicsObject *first{ col.first };
		PhysicsObject *second{ col.second };

		DebugLog::Info("Collision detected between (" +
			std::to_string(first->GetId()) + " " +
			std::to_string(second->GetId()) + ").");

		// Call OnCollision for both physics components.
		first->OnCollision(*second);
		second->OnCollision(*first);
	}

	// Clear collisions list for next frame.
	m_collisions.clear();
}

void PhysicsEngine::UpdateEndpoints()
{
	// 2 endpoints along an axis, for each bounding box interval.
	int numPhysObjs{ static_cast<int>(m_physObjects.size()) };
	int numEndpoints{ 2 * numPhysObjs };

	// If new points were added or points were removed, recreate
	// the endpoints and lookup tables.
	if (m_endpointsX.size() != numEndpoints)
	{
		// Hold the endpoints of all intervals.
		m_endpointsX.resize(numEndpoints);
		m_endpointsY.resize(numEndpoints);

		// Update endpoint based on the physics components list.
		int physObjIndex{ 0 }, endpointIndex{ 0 };
		for (auto it = m_physObjects.begin(); it != m_physObjects.end(); ++it, ++physObjIndex)
		{
			// Update the minimum endpoint of this interval.
			m_endpointsX[endpointIndex].init(physObjIndex, Endpoint::Type::Minimum);
			m_endpointsY[endpointIndex].init(physObjIndex, Endpoint::Type::Minimum);
			++endpointIndex;

			// Update the maximum endpoint of this interval.
			m_endpointsX[endpointIndex].init(physObjIndex, Endpoint::Type::Maximum);
			m_endpointsY[endpointIndex].init(physObjIndex, Endpoint::Type::Maximum);
			++endpointIndex;
		}

		// Update lookup tables.
		m_lookupX.resize(numEndpoints);
		m_lookupY.resize(numEndpoints);
		endpointIndex = 0;
		for (auto it = m_endpointsX.begin(); it != m_endpointsX.end(); ++it, ++endpointIndex)
		{
			int index{ it->getLookupIndex() };
			m_lookupX[index] = endpointIndex;
			m_lookupY[index] = endpointIndex;
		}
	}

	// Update values of each endpoint.
	int physObjIndex{ 0 };
	for (auto it = m_physObjects.begin(); it != m_physObjects.end(); ++it, ++physObjIndex)
	{
		// Get collider values.
		Collider *col{ it->get()->GetCollider() };
		glm::vec3 halfSizes{ col->GetHalfSizes() };
		glm::vec3 pos{ col->GetPosition() };

		// Calculate lookup indices for the minimum and maximum endpoints for
		// this interval.
		int idMin{ 2 * physObjIndex };
		int idMax{ idMin + 1 };

		m_endpointsX[m_lookupX[idMin]].setValue(pos.x - halfSizes.x);
		m_endpointsY[m_lookupY[idMin]].setValue(pos.y - halfSizes.y);
		m_endpointsX[m_lookupX[idMax]].setValue(pos.x + halfSizes.x);
		m_endpointsY[m_lookupY[idMax]].setValue(pos.y + halfSizes.y);
	}
}

void PhysicsEngine::UpdateIntervals(std::vector<Endpoint> &endpoints, 
	std::vector<GLuint> &lookupTable)
{
	for (int i = 0; i < endpoints.size(); ++i)
	{
		// Sort the endpoints by value, in increasing order.
		Endpoint thisPoint{ endpoints[i] };
		int j{ i - 1 };
		while (j >= 0 && thisPoint < endpoints[j] &&
			thisPoint.getValue() != -1)
		{
			Endpoint prevPoint{ endpoints[j] };
			Endpoint nextPoint{ endpoints[j + 1] };

			// No overlap between intervals, so remove it from the overlaps set.
			if (prevPoint.getType() == Endpoint::Type::Minimum &&
				nextPoint.getType() == Endpoint::Type::Maximum)
			{
				int index1{ glm::min(prevPoint.getPhysObjIndex(), nextPoint.getPhysObjIndex()) };
				int index2{ glm::max(prevPoint.getPhysObjIndex(), nextPoint.getPhysObjIndex()) };
				if (index1 != index2)
				{
					m_events.push_back(Event(index1, index2, Event::Type::Remove));
				}
			}
			// Overlap found between intervals.
			else if (prevPoint.getType() == Endpoint::Type::Maximum &&
				thisPoint.getType() == Endpoint::Type::Minimum)
			{
				int index1{ glm::min(prevPoint.getPhysObjIndex(), nextPoint.getPhysObjIndex()) };
				int index2{ glm::max(prevPoint.getPhysObjIndex(), nextPoint.getPhysObjIndex()) };
				if (index1 != index2 &&
					IsOverlapping(index1, index2, m_lookupX, m_endpointsX) &&
					IsOverlapping(index1, index2, m_lookupY, m_endpointsY))
				{
					m_events.push_back(Event(index1, index2, Event::Type::Insert));
				}
			}

			// Swap the endpoints.
			endpoints[j] = nextPoint;
			endpoints[j + 1] = prevPoint;
			lookupTable[nextPoint.getLookupIndex()] = j;
			lookupTable[prevPoint.getLookupIndex()] = j + 1;
			--j;
		}

		endpoints[j + 1] = thisPoint;
		lookupTable[thisPoint.getLookupIndex()] = j + 1;
	}
}

void PhysicsEngine::GenerateOverlapsSet()
{
	// Iterate through events and perform them to get an updated overlaps set.
	for (const Event &event : m_events)
	{
		if (event.getType() == Event::Type::Remove)
		{
			m_overlapsSet.erase(std::make_pair(event.getIndex1(), event.getIndex2()));
		}
		// Insert event.
		else
		{
			m_overlapsSet.insert(std::make_pair(event.getIndex1(), event.getIndex2()));
		}
	}

	// Output the AABBSource of the overlapping endpoints.
	GLuint numPhysObjs{ static_cast<GLuint>(m_physObjects.size()) };
	for (auto it = m_overlapsSet.begin(); it != m_overlapsSet.end();)
	{
		// Prune overlap pairs that include game objects that no longer exist.
		if (it->first >= numPhysObjs || it->second >= numPhysObjs)
		{
			it = m_overlapsSet.erase(it);
		}
		else
		{
			m_collisions.push_back(std::make_pair(
				m_physObjects[it->first].get(),
				m_physObjects[it->second].get()));

			++it;
		}
	}
}

bool PhysicsEngine::IsOverlapping(int endpointIndex1, int endpointIndex2,
	const std::vector<GLuint> &lookupTable,
	const std::vector<Endpoint> &endpoints)
{
	float max1 = endpoints[lookupTable[2 * endpointIndex1 + 1]].getValue();
	float min2 = endpoints[lookupTable[2 * endpointIndex2]].getValue();

	// No overlap if the maximum endpoint of the first interval is at a lower
	// position than the minimum endpoint of the second interval.
	if (max1 < min2)
	{
		return false;
	}

	float min1 = endpoints[lookupTable[2 * endpointIndex1]].getValue();
	float max2 = endpoints[lookupTable[2 * endpointIndex2 + 1]].getValue();
	return min1 <= max2;
}