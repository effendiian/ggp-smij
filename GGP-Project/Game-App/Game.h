#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include "Renderer.h"
#include "MAT_PBRTexture.h"
#include "MAT_Water.h"
#include "LightManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "FirstPersonCamera.h"
#include "ResourceManager.h"
#include "SwimmerManager.h"
#include "Boat.h"

#define NUM_MESHES 4
#define NUM_TEXTURES 12
#define NUM_MATS 3

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
	ResourceManager* resourceManager;
	EntityManager* entityManager;
	SwimmerManager* swimmerManager;

	//Entities
	// std::vector<Entity*> entities; // Refactored into EntityManager.

	//Sampler state
	ID3D11SamplerState* samplerState;

	//Transformation modifiers
	float position;
	float rotation;
	float rotSpeed;
	float scale;

	//Water
	float translate; //used to scroll the water's normal map

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadAssets();
	void CreateEntities();
};

