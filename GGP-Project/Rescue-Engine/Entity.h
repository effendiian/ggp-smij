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
	// Entities are automatically added to the renderlist
	//
	// mesh	- The mesh that this entity displays
	// --------------------------------------------------------
	Entity(Mesh* mesh, Material* material);

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
	// Add this entity to the render list
	// --------------------------------------------------------
	void AddToRenderList();

	// --------------------------------------------------------
	// Remove this entity from the render list
	// --------------------------------------------------------
	void RemoveFromRenderList();

	// --------------------------------------------------------
	// Get the material/mesh identifier
	// --------------------------------------------------------
	std::string GetMatMeshIdentifier();
};

