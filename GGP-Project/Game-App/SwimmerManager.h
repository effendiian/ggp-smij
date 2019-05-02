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
	float maxTTS = 3;
	int maxSwimmerCount;
	const char* swimmerMesh = "Assets\\Models\\swimmer.obj";
	const char* swimmerMat = "swimmer";
	std::mt19937 rng;
	float levelRadius;

	//Singleton
	SwimmerManager();
	~SwimmerManager();

	// Generates random position that's in bounds.
	DirectX::XMFLOAT3 GetNextPosition();
	// DirectX::XMFLOAT3 GetNextRotation();
	// DirectX::XMFLOAT3 GetNextScale();

	// Randomizes swimmer configuration at spawn.
	void RandomizeSwimmer(Swimmer* swimmer);

	std::vector<Swimmer*> swimmers;

public: // PUBLIC --------------------------------------

	// Singleton.
	static SwimmerManager* GetInstance()
	{
		static SwimmerManager instance;
		return &instance;
	}
	//Delete this
	SwimmerManager(SwimmerManager const&) = delete;
	void operator=(SwimmerManager const&) = delete;

	// --------------------------------------------------------
	// Check if the swimmer manager is now ready to spawn.
	// --------------------------------------------------------
	bool IsReadyToSpawn();

	// --------------------------------------------------------
	// Reset manager.
	// --------------------------------------------------------
	void Reset();

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
	Swimmer* GetSwimmer(int index);

	// --------------------------------------------------------
	// Get swimmer count
	// --------------------------------------------------------
	int GetSwimmerCount();

	// --------------------------------------------------------
	// Set level radius
	// --------------------------------------------------------
	void SetLevelRadius(float radius);
		
	// --------------------------------------------------------
	// Attach swimmer to a leader.
	// --------------------------------------------------------
	void AttachSwimmer(Swimmer* swimmer, Entity* leader, int index);
};