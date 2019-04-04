#include "EntityManager.h"

//Initializes the Entity Manager.
void EntityManager::Init()
{
	entities_count = 0;
}

//Releases the entities in the Entity Manager.
void EntityManager::Release()
{
	for (auto i = 0; i < entities_count; i++)
	{
		delete[] entities[i];
	}
}

//Adds an entity to the Entity Manager with a unique ID.
void EntityManager::AddEntity(Entity* e, std::string id)
{
	entities.push_back(e);
	entity_ids.push_back(id);
	entities_count++;
}

//Gets an entity from the Entity Manager with a unique ID.
Entity* EntityManager::GetEntity(std::string id)
{
	for (auto i = 0; i < entities_count; i++)
	{
		if (entity_ids[i] == id) {
			return GetEntity(i);
		}
	}

	return nullptr;
}

//Gets an entity from the Entity Manager with an index.
Entity* EntityManager::GetEntity(int index)
{
	return entities[index];
}

//Removes an entity from the Entity Manager with a unique ID.
void EntityManager::RemoveEntity(std::string id)
{
	for (auto i = 0; i < entities_count; i++)
	{
		if (entity_ids[i] == id)
		{
			RemoveEntity(i);
		}
	}
}

//Removes an entity from the Entity Manager with an index.
void EntityManager::RemoveEntity(int index)
{
	if (index >= 0 && index < entities_count)
	{
		entities.erase(entities.begin() + index);
		entity_ids.erase(entity_ids.begin() + index);
		entities_count--;
	}
}

//Updates the entities in the Entity Manager.
void EntityManager::Update()
{
	for (auto i = 0; i < entities_count; i++)
	{
		std::string name = entity_ids[i];

		//Update objects here
		//if(name == "boat")
		//   entities[i].translate etc...
	}
}