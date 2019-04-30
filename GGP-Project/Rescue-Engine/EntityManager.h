#pragma once
#include <vector>
#include <Entity.h>
#include <string>

struct EntityRemoval {
	Entity* e;
	bool release;
};

class EntityManager
{
private:
	// --------------------------------------------------------
	// Singleton Constructor - Set up the singleton instance of the EntityManager
	// --------------------------------------------------------
	EntityManager() { }
	~EntityManager();

	std::vector<Entity*> entities;       //A vector of entities
	std::vector<EntityRemoval> remove_entities;       //A vector of entities

	// --------------------------------------------------------
	// Remove an entity by its object
	// --------------------------------------------------------
	void RemoveEntityFromList(Entity* entity, bool release);

public:

	// Returns an Entity Manager Instance ---
	static EntityManager* GetInstance()
	{
		static EntityManager instance;
		return &instance;
	}

	//Delete this
	EntityManager(EntityManager const&) = delete;
	void operator=(EntityManager const&) = delete;

	// Entity Methods -----------------------

	// --------------------------------------------------------
	// Add an entity to the entity manager
	// (checks if it is already in it)
	// --------------------------------------------------------
	void AddEntity(Entity* entity);

	// --------------------------------------------------------
	// Get an entity by its name
	// --------------------------------------------------------
	Entity* GetEntity(std::string name);

	// --------------------------------------------------------
	// Remove an entity by its name
	// --------------------------------------------------------
	void RemoveEntity(std::string name, bool deleteEntity = true);

	// --------------------------------------------------------
	// Remove an entity by its object
	// --------------------------------------------------------
	void RemoveEntity(Entity* entity, bool deleteEntity = true);

	// --------------------------------------

	// --------------------------------------------------------
	// Run Update() for all entities in the manager
	// --------------------------------------------------------
	void Update(float deltaTime);
};