#pragma once
#include <DirectXMath.h>
#include "Entity.h"

//Enum for swimmer states
enum class SwimmerState { Entering, Floating, Joining, Following, Still, Hitting, Nothing };

class Swimmer :
	public Entity
{
private:

	//Follow state vars
	SwimmerState swmrState;
	static const int MAX_FPS = 60;
	Entity* leader;
	float lagSeconds = 0.7f;
	float hitTimer;

	//Snake movement buffer vars
	DirectX::XMFLOAT3* positionBuffer;
	float* timeBuffer;
	int bufferLength;
	int oldestIndex;
	int newestIndex;
	float timer;

	//Buoyancy vars
	float velocity;
	float acceleration;
	float sinAmnt;
	float gravityMult;

	// --------------------------------------------------------
	// Run this swimmer's entering behaviour
	//---------------------------------------------------------
	void Enter(float deltaTime);

	// --------------------------------------------------------
	// Apply buoyancy to the swimmer
	//---------------------------------------------------------
	void ApplyBuoyancy(float deltaTime);

	// --------------------------------------------------------
	// Apply buoyancy to the swimmer
	//---------------------------------------------------------
	void ApplyCos(float deltaTime);

	// --------------------------------------------------------
	// Run this swimmer's floating behaviour
	//---------------------------------------------------------
	void Float(float deltaTime);

	// --------------------------------------------------------
	// Update the swimmer's buffers for snake movement
	// --------------------------------------------------------
	DirectX::XMFLOAT3 GetTrailPos(float deltaTime);

	// --------------------------------------------------------
	// Causes this swimmer to seek the surface y's position
	// --------------------------------------------------------
	void SeekSurfaceY();

	// --------------------------------------------------------
	// Run this swimmer's joining behaviour
	// --------------------------------------------------------
	void Join(float deltaTime);

	// --------------------------------------------------------
	// Run this swimmer's following behaviour
	// --------------------------------------------------------
	void Follow(float deltaTime);

	// --------------------------------------------------------
	// Run this swimmer's hitting behaviour
	// --------------------------------------------------------
	void Hit(float deltaTime);

public:
	Swimmer(Mesh* mesh, Material* material, std::string name);
	~Swimmer();

	// --------------------------------------------------------
	// Control which movement the swimmer is performing
	// --------------------------------------------------------
	void Update(float deltaTime);

	// --------------------------------------------------------
	// Check if the boat is in the following state
	// --------------------------------------------------------
	bool IsFollowing();

	// --------------------------------------------------------
	// Set Swimmer to follow a game object.
	// --------------------------------------------------------
	void JoinTrail(Entity* leader);

	// --------------------------------------------------------
	// Check if the swimmer is in the hitting state for the correct amount of time	// --------------------------------------------------------
	bool CheckHit();

	// --------------------------------------------------------
	// Set Swimmer's state to a new state
	// --------------------------------------------------------
	void SetSwimmerState(SwimmerState newState);
};

