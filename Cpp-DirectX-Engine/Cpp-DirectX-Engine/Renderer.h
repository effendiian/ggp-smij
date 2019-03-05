#pragma once
#include <vector>
#include "SimpleShader.h"
#include "Entity.h"
#include "Camera.h"
#include "Lights.h"

// Basis from: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern

// --------------------------------------------------------
// Singleton
//
// Handles rendering entities to the screen
// --------------------------------------------------------
class Renderer
{
private:
	std::vector<Entity*> renderList;
	
	//Test lighting
	DirectionalLight* dLight;
	PointLight* pLight;
	SpotLight* sLight;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// --------------------------------------------------------
	// Singleton Constructor - Set up the singleton instance of the renderer
	// --------------------------------------------------------
	Renderer() { Init(); }

	// --------------------------------------------------------
	// Destructor for when the singleton instance is deleted
	// --------------------------------------------------------
	~Renderer();

	// --------------------------------------------------------
	// Initialize values in the renderer
	// --------------------------------------------------------
	void Init();

public:
	// --------------------------------------------------------
	// Get the singleton instance of the renderer
	// --------------------------------------------------------
	static Renderer* GetInstance()
	{
		static Renderer instance;

		return &instance;
	}

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
	void AddEntityToRenderList(Entity* e);

	// --------------------------------------------------------
	// Remove an entity from the render list
	// --------------------------------------------------------
	void RemoveEntityFromRenderList(Entity* e);

	// --------------------------------------------------------
	// Check if an entity is in the render list. O(n) complexity
	// --------------------------------------------------------
	bool IsEntityInRenderList(Entity* e);
};