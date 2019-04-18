#include "EntityManager.h"

//Releases the entities in the Entity Manager.
EntityManager::~EntityManager()
{
	for (auto i = 0; i < entities.size(); i++)
	{
		if (entities[i]) { delete[] entities[i]; }
	}
}

//Adds an entity to the Entity Manager with a unique ID.
void EntityManager::AddEntity(Entity* e)
{
	//Check the iterator of the entity
	if (std::find(entities.begin(), entities.end(), e) != entities.end())
	{
		printf("Cannot add entity %s because it is already in entity manager", e->GetName().c_str());
		return;
	}

	//Add to the list
	entities.push_back(e);
}

//Gets an entity from the Entity Manager with a certain name.
Entity* EntityManager::GetEntity(std::string id)
{
	for (auto i = 0; i < entities.size(); i++)
	{
		if (entities[i]->GetName() == id) {
			return entities[i];
		}
	}

	return nullptr;
}

// Remove an entity by its object
void EntityManager::RemoveEntityFromList(Entity* entity, bool deleteEntity)
{
	Entity* org = entity;

	//Get the iterator of the entity
	//Swap it for the last one
	std::swap(entity, entities[entities.size() - 1]);

	//Pop the last one
	entities.pop_back();

	//Delete instance if user wants to
	if (deleteEntity)
		delete org;

	return;
}

// Remove an entity by its name
void EntityManager::RemoveEntity(std::string name, bool deleteEntity)
{
	for (auto i = 0; i < entities.size(); i++)
	{
		if (entities[i]->GetName() == name)
		{
			Entity* e = entities[i];
			e->SetEnabled(false);
			remove_entities.push_back(EntityRemoval{ e, deleteEntity });
			return;
		}
	}

	printf("Entity of name %s does not exist in EntityManager. Cannot remove", name.c_str());
}

// Remove an entity by its object
void EntityManager::RemoveEntity(Entity* entity, bool deleteEntity)
{
	//Get the iterator of the entity
	std::vector<Entity*>::iterator it = std::find(entities.begin(), entities.end(), entity);
	if (it == entities.end())
	{
		printf("Cannot remove entity %s because it is not in entity manager", entity->GetName().c_str());
		return;
	}

	entity->SetEnabled(false);
	remove_entities.push_back(EntityRemoval{entity, deleteEntity});
	return;
}

// Run Update() for all entities in the manager
void EntityManager::Update(float deltaTime)
{
	//Update entities
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (entities[i]->GetEnabled())
		{
			entities[i]->GameObject::Update(deltaTime);
			entities[i]->Update(deltaTime);
		}
	}

	//Remove entities
	for (auto const& er : remove_entities)
	{
		RemoveEntityFromList(er.e, er.release);
	}
	remove_entities.clear();
}