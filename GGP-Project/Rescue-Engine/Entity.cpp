#include "Renderer.h"
#include <sstream> 
#include <string> 

// For the DirectX Math library
using namespace DirectX;

// Constructor - Set up the entity
Entity::Entity(Mesh* mesh, Material* material)
{
	this->mesh = mesh;
	this->material = material;

	//Create a unique identifer (combination of the two addresses).
	//	Used in the renderer
	const void * addressMat = static_cast<const void*>(material);
	const void * addressMesh = static_cast<const void*>(mesh);
	std::stringstream ss;
	ss << addressMat << addressMesh;
	identifier = ss.str().c_str;

	Renderer::GetInstance()->AddEntityToRenderList(this);
}

// Destructor for when an instance is deleted
Entity::~Entity()
{ 
	Renderer::GetInstance()->RemoveEntityFromRenderList(this);
}

// Get the material this entity uses
Material* Entity::GetMaterial()
{
	return material;
}

// Get the mesh this entity uses
Mesh* Entity::GetMesh()
{
	return mesh;
}

// Add this entity to the render list
void Entity::AddToRenderList()
{
	Renderer::GetInstance()->AddEntityToRenderList(this);
}

// Remove this entity from the render list
void Entity::RemoveFromRenderList()
{
	Renderer::GetInstance()->RemoveEntityFromRenderList(this);
}

// Get the material/mesh identifier
char* Entity::GetMatMeshIdentifier()
{
	return identifier;
}