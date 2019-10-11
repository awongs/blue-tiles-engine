#pragma once

#include <glad/glad.h>

#include <memory>
#include <set>
#include <vector>
#include <string>

class Collider;
class SphereCollider;
class PhysicsObject;

class PhysicsEngine
{
	class Endpoint
	{
	public:
		// Defines whether the endpoint is a minimum or maximum point.
		enum class Type
		{
			MINIMUM,
			MAXIMUM
		};

		inline void init(int physObjIndex, Type type)
		{
			m_physObjIndex = physObjIndex;
			m_type = type;
			m_value = -1;
		}

		inline void setValue(float val)
		{
			m_value = val;
		}

		inline int getPhysObjIndex() const
		{
			return m_physObjIndex;
		}

		inline int getLookupIndex() const
		{
			return 2 * m_physObjIndex + (m_type == Type::MAXIMUM ? 1 : 0);
		}

		inline Type getType() const
		{
			return m_type;
		}

		inline float getValue() const
		{
			return m_value;
		}

		bool operator < (const Endpoint &other) const
		{
			if (m_value == other.m_value)
			{
				return m_type < other.getType();
			}
			else
			{
				return m_value < other.getValue();
			}
		};

	private:
		// The index in the list of physics components that this 
		// endpoint belongs to.
		int m_physObjIndex;

		// The position value of the endpoint in one axis.
		float m_value;

		// A minimum or maximum endpoint.
		Type m_type;
	};

	class Event
	{
	public:
		// Instruction type: either removing or inserting
		// elements from/to the set of overlapping intervals.
		enum class Type
		{
			REMOVE,
			INSERT
		};

		Event(int index1, int index2, Type type) :
			m_index1(index1), m_index2(index2), m_type(type)
		{

		};

		inline Type getType() const
		{
			return m_type;
		}

		inline int getIndex1() const
		{
			return m_index1;
		}

		inline int getIndex2() const
		{
			return m_index2;
		}

	private:
		// The event's instruction type.
		Type m_type;

		// The indices of the 2 physics components involved.
		int m_index1;
		int m_index2;
	};

public:
	// Check for collisions between each of the physics components.
	void Update();

	// Add a physics component for a game object to the vector.
	PhysicsObject *AddPhysicsObject();

	static const std::string COLLISION_MESSAGE_STR;

private:
	// Roughly check for potential collisions using bounding boxes.
	void DoBroadPhase();

	// Detect collisions more precisely, given that they were
	// detected during broad phase.
	void DoNarrowPhase();

	// Handle all detected collisions.
	void HandleCollisions();

	// Update the position values in the list of endpoints with the 
	// values of the list of physics components.
	void UpdateEndpoints();

	// Sort the endpoints and produce events to update the set of
	// overlapping intervals. Intervals are defined by the area bounded
	// by their 2 endpoints in one axis.
	void UpdateIntervals(std::vector<Endpoint> &endpoints, 
		std::vector<GLuint> &lookupTable);

	// Generate the list of overlapping intervals from the set of instructions
	// created produced during the "sort" phase.
	void GenerateOverlapsSet();

	// Check if two endpoints are overlapping.
	bool IsOverlapping(int endpointIndex1, int endpointIndex2,
		const std::vector<GLuint> &lookupTable,
		const std::vector<Endpoint> &endpoints);

	// Narrow phase: check if 2 spheres are colliding.
	bool IsSphereSphereColliding(SphereCollider *col1, SphereCollider *col2);

	// Narrow phase: check if a sphere and a box are colliding.
	bool IsBoxSphereColliding(Collider *col1, SphereCollider *col2);

	// Hold all the physics components.
	std::vector<std::unique_ptr<PhysicsObject>> m_physObjects;

	// Hold all endpoints in the x and y-directions, sorted in ascending order.
	// Endpoints refer to the bounds of an AABB, projected onto an axis.
	std::vector<Endpoint> m_endpointsX;
	std::vector<Endpoint> m_endpointsY;

	// Lookup tables for endpoints in the x and y-directions.
	// Since the vector of endpoints is sorted, the endpoint's index in that
	// vector is not stable. So we need a fixed way to determine an
	// endpoint's index.
	// Indices of the lookup table are determined by the index of the
	// physics component that the endpoint belongs to.
	// Vector elements correspond to indices for the list of endpoints.
	std::vector<GLuint> m_lookupX;
	std::vector<GLuint> m_lookupY;

	// Hold a list of actions to perform on the set of overlapping intervals.
	// Elements may be removed or inserted.
	std::vector<Event> m_events;

	// Hold all pairs of game object ids with overlapping intervals.
	// The paired game objects are close enough to each other to warrant 
	// a narrow phase check.
	std::set<std::pair<GLuint, GLuint>> m_overlapsSet;

	// Hold pairs of physics components that collided during broad phase 
	// for this frame.
	std::vector<std::pair<PhysicsObject *, PhysicsObject *>> m_broadCollisions;

	// Hold pairs of physics components that collided after narrow phase
	// for this frame.
	std::vector<std::pair<PhysicsObject *, PhysicsObject *>> m_collisions;
};