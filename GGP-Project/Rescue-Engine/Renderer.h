#pragma once
#include <vector>
#include <unordered_map>
#include "SimpleShader.h"
#include "Entity.h"
#include "Camera.h"
#include "FXAA.h"

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
	std::unordered_map<std::string, std::vector<Entity*>> renderMap;

	//Collider debugging
	std::vector<Collider*> debugColliders;
	Mesh* colDebugCube;
	SimpleVertexShader* colDebugVS;
	SimplePixelShader* colDebugPS;

	//Raster states
	ID3D11RasterizerState* RS_wireframe;

	// Post-Process: FXAA ------------------
	ID3D11RenderTargetView* fxaaRTV; // Allow us to render to a texture.
	ID3D11ShaderResourceView* fxaaSRV; // Allow us to sample from the same texture.
	SimpleVertexShader* fxaaVS;
	SimplePixelShader* fxaaPS;
	FXAA_DESC* fxaaSettings;

	// Clear color.
	float clearColor[4];

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
	void Init(ID3D11Device* device, UINT width, UINT height);

	//Delete this
	Renderer(Renderer const&) = delete;
	void operator=(Renderer const&) = delete;

	// --------------------------------------------------------
	// Draw all entities in the render list
	//
	// context - DirectX device context
	// camera - The active camera object
	// --------------------------------------------------------
	void Draw(ID3D11DeviceContext* context,
			  Camera* camera,
			  ID3D11RenderTargetView* backBufferRTV,
		      ID3D11DepthStencilView* depthStencilView,
			  ID3D11SamplerState* sampler,
			  UINT width,
		      UINT height
	);

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

	// --------------------------------------------------------
	// Set clear color.
	// --------------------------------------------------------
	void SetClearColor(const float color[4]);
	void SetClearColor(float r, float g, float b, float a = 1.0);
};