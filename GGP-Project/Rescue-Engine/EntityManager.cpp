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

// Enable the entity.
void EntityManager::EnableEntity(std::string entity_id) 
{
	Entity* entity = this->GetEntity(entity_id);
	if (entity != nullptr) {
		entity->Enable = true;
	}
}

// Disable the entity.
void EntityManager::DisableEntity(std::string entity_id)
{
	Entity* entity = this->GetEntity(entity_id);
	if (entity != nullptr) {
		entity->Enable = false;
	}
}

//Updates all of the entities in the Entity Manager.
void EntityManager::Update(float deltaTime)
{
	for (auto i = 0; i < entities_count; i++)
	{
		if (entities[i]->Enable)
		{
			for (auto j = 0; j < entities_count; j++)
			{
				if (i != j && entities[j]->Enable && entities[i]->GetCollider() != nullptr && entities[j]->GetCollider())
				{
					if (entities[i]->GetCollider()->Collides(*entities[j]->GetCollider()))
					{
						printf("collision!");
					}
				}
			}
		}
	}
}

//Updates specific entity in the Entity Manager.
void EntityManager::Update(float deltaTime, std::string entityId)
{
	for (auto i = 0; i < entities_count; i++)
	{
		if (entityId == entity_ids[i] && entities[i]->Enable) 
		{
			entities[i]->Update(deltaTime);
		}
	}
}