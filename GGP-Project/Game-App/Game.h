#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include "Renderer.h"
#include "MAT_PBRTexture.h"
#include "InputManager.h"
#include "FirstPersonCamera.h"
#include "WICTextureLoader.h"

#define NUM_MESHES 4
#define NUM_TEXTURES 12
#define NUM_MATS 3
#define NUM_ENTITIES 5

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	//Camera
	FirstPersonCamera* camera;

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y, int button);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:
	//Singletons
	Renderer* renderer;
	InputManager* inputManager;

	//Meshes and entities
	Mesh* meshes[NUM_MESHES];
	Entity* entities[NUM_ENTITIES];

	//Materials and textures
	Material* materials[NUM_MATS];
	ID3D11SamplerState* samplerState;
	ID3D11ShaderResourceView* texture_srvs[NUM_TEXTURES];

	//Transformation modifiers
	float position;
	float rotation;
	float rotSpeed;
	float scale;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders();
	void LoadAssets();
	void CreateEntities();
};

