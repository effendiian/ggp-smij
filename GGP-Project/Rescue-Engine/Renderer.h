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
	Mesh* cubeMesh;

	//Collider debugging
	std::vector<DirectX::XMFLOAT4X4> debugCubes;
	SimpleVertexShader* vs_debug;
	SimplePixelShader* ps_debug;
	ID3D11RasterizerState* RS_wireframe;

	//Water
	Material* waterMat;
	Entity* water;
	ID3D11BlendState* waterBlendState;
	ID3D11DepthStencilState* waterDepthState;

	//Skybox
	Material* skyboxMat;
	ID3D11RasterizerState* skyRasterState;
	ID3D11DepthStencilState* skyDepthState;



	//Shadows
	ID3D11RasterizerState* shadowRasterizer;
	SimpleVertexShader* shadowVS;

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

	// --------------------------------------------------------
	// Prepare post-process render texture.
	// --------------------------------------------------------
	void PreparePostProcess(ID3D11DeviceContext* context, 
		ID3D11RenderTargetView* ppRTV, 
		ID3D11DepthStencilView* ppDSV);

	// --------------------------------------------------------
	// Render shadow maps for all lights that cast shadows
	// --------------------------------------------------------
	void RenderShadowMaps(ID3D11DeviceContext* context,
		ID3D11Device* device,
		Camera* camera,
		ID3D11RenderTargetView* backBufferRTV,
		ID3D11DepthStencilView* depthStencilView,
		UINT width, UINT height);

	// --------------------------------------------------------
	// Draw opaque objects
	// --------------------------------------------------------
	void DrawOpaqueObjects(ID3D11DeviceContext* context, Camera* camera);

	// --------------------------------------------------------
	// Draw transparent water
	// --------------------------------------------------------
	void DrawWater(ID3D11DeviceContext* context, Camera* camera);

	// --------------------------------------------------------
	// Draw debug colider rectangles
	// --------------------------------------------------------
	void DrawDebugColliders(ID3D11DeviceContext* context, Camera* camera);

	// --------------------------------------------------------
	// Draw the skybox
	// --------------------------------------------------------
	void DrawSky(ID3D11DeviceContext* context, Camera* camera);

	// --------------------------------------------------------
	// Apply post processing.
	// --------------------------------------------------------
	void ApplyPostProcess(ID3D11DeviceContext* context,
		ID3D11RenderTargetView* backBufferRTV,
		ID3D11DepthStencilView* depthStencilView,
		ID3D11RenderTargetView* ppRTV,
		ID3D11ShaderResourceView* ppSRV,
		ID3D11SamplerState* sampler,
		UINT width, UINT height);

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
			  ID3D11Device* device,
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
	void AddDebugCubeToThisFrame(DirectX::XMFLOAT3 position, float size = 1);

	// --------------------------------------------------------
	// Tell the renderer to render a collider this frame
	// --------------------------------------------------------
	void AddDebugCubeToThisFrame(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 rotation, float size = 1);

	// --------------------------------------------------------
	// Tell the renderer to render a collider this frame
	// --------------------------------------------------------
	void AddDebugCubeToThisFrame(DirectX::XMFLOAT4X4 world);

	// --------------------------------------------------------
	// Set clear color.
	// --------------------------------------------------------
	void SetClearColor(const float color[4]);
	void SetClearColor(float r, float g, float b, float a = 1.0);

	// --------------------------------------------------------
	// Create the post-processing texture
	// --------------------------------------------------------
	void CreatePostProcessingResources(ID3D11Device* device, UINT width, UINT height);
};