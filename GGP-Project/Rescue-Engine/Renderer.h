#pragma once
#include <vector>
#include <map>
#include "SimpleShader.h"
#include "Entity.h"
#include "Camera.h"

// Basis from: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern

// --------------------------------------------------------
// Singleton
//
// Handles rendering entities to the screen
// --------------------------------------------------------
class Renderer
{
private:
	//Render list management
	//renderMap uses Mat/Mesh identifiers to point to the correct list
	std::map<std::string, std::vector<Entity*>> renderMap;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	//Collider debugging
	std::vector<Collider*> debugColliders;
	Mesh* colDebugCube;
	SimpleVertexShader* colDebugVS;
	SimplePixelShader* colDebugPS;

	//Raster states
	ID3D11RasterizerState* RS_wireframe;

	// --------------------------------------------------------
	// Singleton Constructor - Set up the singleton instance of the renderer
	// --------------------------------------------------------
	Renderer() {}

	// --------------------------------------------------------
	// Destructor for when the singleton instance is deleted
	// --------------------------------------------------------
	~Renderer();

public:
	// --------------------------------------------------------
	// Get the singleton instance of the renderer
	// --------------------------------------------------------
	static Renderer* GetInstance()
	{
		static Renderer instance;

		return &instance;
	}

	// --------------------------------------------------------
	// Initialize values in the renderer
	// --------------------------------------------------------
	void Init(ID3D11Device* device);

	//Delete this
	Renderer(Renderer const&) = delete;
	void operator=(Renderer const&) = delete;

	// --------------------------------------------------------
	// Draw all entities in the render list
	//
	// context - DirectX device context
	// camera - The active camera object
	// --------------------------------------------------------
	void Draw(ID3D11DeviceContext* context, Camera* camera);

	// --------------------------------------------------------
	// Add an entity to the render list
	// --------------------------------------------------------
	void AddEntityToRenderer(Entity* e);

	// --------------------------------------------------------
	// Remove an entity from the render list
	// --------------------------------------------------------
	void RemoveEntityFromRenderer(Entity* e);

	// --------------------------------------------------------
	// Check if an entity is in the render list. O(n) complexity
	// --------------------------------------------------------
	bool IsEntityInRenderer(Entity* e);

	// --------------------------------------------------------
	// Tell the renderer to render a collider this frame
	// --------------------------------------------------------
	void RenderColliderThisFrame(Collider* c);
};