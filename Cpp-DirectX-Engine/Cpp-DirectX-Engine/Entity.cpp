#include "Renderer.h"

// For the DirectX Math library
using namespace DirectX;

// Constructor - Set up the entity
Entity::Entity(Mesh* mesh, Material* material)
{
	this->mesh = mesh;
	this->material = material;

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

// Get the vertex shader this entity's material uses
SimpleVertexShader* Entity::GetVertexShader()
{
	return material->GetVertexShader();
}

// Get the pixel this entity's material uses
SimplePixelShader* Entity::GetPixelShader()
{
	return material->GetPixelShader();
}

// Get the mesh this entity uses
Mesh* Entity::GetMesh()
{
	return mesh;
}

// Get the vertex buffer the entity's mesh uses
ID3D11Buffer* Entity::GetVertexBuffer()
{
	return mesh->GetVertexBuffer();
}

// Get the index buffer the entity's mesh uses
ID3D11Buffer* Entity::GetIndexBuffer()
{
	return mesh->GetIndexBuffer();
}

// Get the number of indicies this entity's mesh uses
int Entity::GetIndexCount()
{
	return mesh->GetIndexCount();
}

// Add this entity to the render list
void Entity::AddThisEntityToRenderList()
{
	Renderer::GetInstance()->AddEntityToRenderList(this);
}

// Remove this entity from the render list
void Entity::RemoveThisEntityFromRenderList()
{
	Renderer::GetInstance()->RemoveEntityFromRenderList(this);
}