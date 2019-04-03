#pragma once
#include <vector>
#include <Entity.h>
#include <string>

class EntityManager
{

public: // PUBLIC --------------------------------------------------------
	
	// Main Instantiation Methods -----------

	void Init();
	void Release();

	// --------------------------------------

	// Entity Methods -----------------------

	void AddEntity(Entity*, std::string);
	Entity* GetEntity(std::string);
	Entity* GetEntity(int);
	void RemoveEntity(std::string);
	void RemoveEntity(int);

	// --------------------------------------

	// Update Methods -----------------------

	void Update();

	// --------------------------------------

	// Returns an Entity Manager Instance ---

	static EntityManager* GetInstance()
	{
		static EntityManager instance;
		return &instance;
	} 

	// --------------------------------------

private: // PRIVATE ------------------------------------------------------

	std::vector<std::string> entity_ids; //A vector of entity id's (strings)
	std::vector<Entity*> entities;       //A vector of entities
	int entities_count;                  //The amount of entities in the manager
	
};