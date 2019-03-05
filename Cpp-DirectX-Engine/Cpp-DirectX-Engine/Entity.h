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
	// Get the vertex shader this entity's material uses
	// --------------------------------------------------------
	SimpleVertexShader* GetVertexShader();

	// --------------------------------------------------------
	// Get the pixel this entity's material uses
	// --------------------------------------------------------
	SimplePixelShader* GetPixelShader();
	
	// --------------------------------------------------------
	// Get the mesh this entity uses
	// --------------------------------------------------------
	Mesh* GetMesh();

	// --------------------------------------------------------
	// Get the vertex buffer the entity's mesh uses
	// --------------------------------------------------------
	ID3D11Buffer* GetVertexBuffer();

	// --------------------------------------------------------
	// Get the index buffer the entity's mesh uses
	// --------------------------------------------------------
	ID3D11Buffer* GetIndexBuffer();

	// --------------------------------------------------------
	// Get the number of indicies this entity's mesh uses
	// --------------------------------------------------------
	int GetIndexCount();

	// --------------------------------------------------------
	// Add this entity to the render list
	// --------------------------------------------------------
	void AddThisEntityToRenderList();

	// --------------------------------------------------------
	// Remove this entity from the render list
	// --------------------------------------------------------
	void RemoveThisEntityFromRenderList();
};

