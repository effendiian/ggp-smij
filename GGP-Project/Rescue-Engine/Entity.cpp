#include "Renderer.h"
#include "EntityManager.h"
#include <sstream> 

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
	std::string temp = ss.str();
	identifier = ss.str();


	Renderer::GetInstance()->AddEntityToRenderer(this);
	EntityManager::GetInstance()->AddEntity(this);
}

// Constructor - Set up the entity.
Entity::Entity(Mesh * mesh, Material * material, std::string name)
	: Entity(mesh, material)
{
	this->name = name;
}

// Destructor for when an instance is deleted
Entity::~Entity()
{ 
	Renderer::GetInstance()->RemoveEntityFromRenderer(this);
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

// Get the material/mesh identifier
std::string Entity::GetMatMeshIdentifier()
{
	return identifier;
}