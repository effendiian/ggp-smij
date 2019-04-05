#pragma once

#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"

// --------------------------------------------------------
// A entity definition.
//
// An entity is a game object that renders a model
// --------------------------------------------------------
class Entity : public GameObject
{
private:
	//Rendering
	Mesh* mesh;
	Material* material;
	std::string identifier;

public:
	// --------------------------------------------------------
	// Constructor - Set up the entity.
	// Entities are automatically added to the EntityManager and Renderer
	//
	// mesh	- The mesh that this entity displays
	// material - The material this entity uses.
	// --------------------------------------------------------
	Entity(Mesh* mesh, Material* material);

	// --------------------------------------------------------
	// Constructor - Set up the entity.
	// Entities are automatically added to the EntityManager and Renderer
	//
	// mesh	- The mesh that this entity displays
	// material - The material this entity uses.
	// name - The name of the entity
	// --------------------------------------------------------
	Entity(Mesh* mesh, Material* material, std::string name);

	// --------------------------------------------------------
	// Destructor for when an instance is deleted
	// --------------------------------------------------------
	~Entity();

	// --------------------------------------------------------
	// Get the material this entity uses
	// --------------------------------------------------------
	Material* GetMaterial();
	
	// --------------------------------------------------------
	// Get the mesh this entity uses
	// --------------------------------------------------------
	Mesh* GetMesh();

	// --------------------------------------------------------
	// Get the material/mesh identifier
	// --------------------------------------------------------
	std::string GetMatMeshIdentifier();
};