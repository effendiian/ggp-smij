#pragma once
#include <DirectXMath.h>
#include "Entity.h"
#include "Swimmer.h"
#include <random>

class SwimmerManager :
	public GameObject
{
private: // PRIVATE ------------------------------------

	float currentTTS = 0;
	int swimmerCount = 0;
	int maxSwimmerCount = 10;
	const char* swimmerMesh = "Assets\\Models\\sphere.obj";
	const char* swimmerMat = "wood";
	std::mt19937 rng;

	// Generates random position that's in bounds.
	DirectX::XMFLOAT3 GetNextPosition();
	// DirectX::XMFLOAT3 GetNextRotation();
	// DirectX::XMFLOAT3 GetNextScale();

	// Randomizes swimmer configuration at spawn.
	void RandomizeSwimmer(Swimmer* swimmer);

public: // PUBLIC --------------------------------------
	SwimmerManager();
	SwimmerManager(DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset);
	SwimmerManager(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset);
	~SwimmerManager();

	// --------------------------------------------------------
	// Check if the swimmer manager is now ready to spawn.
	// --------------------------------------------------------
	bool IsReadyToSpawn();

	// --------------------------------------------------------
	// Reset manager.
	// --------------------------------------------------------
	void Reset();

	// --------------------------------------------------------
	// Increase the number of maximum swimmers.
	//
	// Should be called after set is returned to the dock.
	// --------------------------------------------------------
	void IncreaseLevel();

	// --------------------------------------------------------
	// Updates swimmers and spawn time.
	// --------------------------------------------------------
	void Update(float deltaTime);

	// --------------------------------------------------------
	// Create a swimmer and spawn at a random position.
	// --------------------------------------------------------
	Swimmer* SpawnSwimmer();

	// --------------------------------------------------------
	// Find swimmer based on swimmer count.
	// --------------------------------------------------------
	Swimmer* GetSwimmer(int);
		
	// --------------------------------------------------------
	// Attach swimmer to the input object.
	// --------------------------------------------------------
	void AttachSwimmer(Swimmer* swimmer, GameObject* object);

	// --------------------------------------------------------
	// Detach swimmer from all objects.
	// --------------------------------------------------------
	void DetachSwimmer(Swimmer* swimmer);

	// Read-only accessors.
	int const& SwimmerCount = swimmerCount;
	int const& MaxSwimmerCount = maxSwimmerCount;
	float const& TTS = 5; // time-to-spawn.

	// Singleton.
	static SwimmerManager* GetInstance() 
	{
		static SwimmerManager instance;
		return &instance;
	}

};