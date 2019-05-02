#pragma once
#include <DirectXMath.h>
#include "Entity.h"

//Enum for swimmer states
enum class SwimmerState { Entering, Floating, Joining, Following, Still, Hitting, Nothing, Leaving };

class Swimmer :
	public Entity
{
private:

	//Follow state vars
	SwimmerState swmrState;
	static const int MAX_FPS = 60;
	Entity* leader;
	float lagSeconds = 0.5f;
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
	void ApplyWaterPhysics(float deltaTime, bool applyBuoyancy);

	// --------------------------------------------------------
	// Run this swimmer's floating behaviour
	//---------------------------------------------------------
	void Float(float deltaTime);

	// --------------------------------------------------------
	// Update the swimmer's buffers for snake movement
	// --------------------------------------------------------
	DirectX::XMFLOAT3 GetTrailPos(float deltaTime);

	// --------------------------------------------------------
	// Get the rotation for following on the trail
	// --------------------------------------------------------
	DirectX::XMFLOAT4 GetTrailRotation(float deltaTime);

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

	// --------------------------------------------------------
	// Run this swimmer's leaving behaviour
	// --------------------------------------------------------
	void Leave(float deltaTime);

public:
	Swimmer(Mesh* mesh, Material* material, std::string name);
	~Swimmer();

	// --------------------------------------------------------
	// Control which movement the swimmer is performing
	// --------------------------------------------------------
	void Update(float deltaTime);

	// --------------------------------------------------------
	// Set Swimmer to follow a game object.
	// --------------------------------------------------------
	void JoinTrail(Entity* leader);

	// --------------------------------------------------------
	// Check if the swimmer is in the hitting state for the correct amount of time	
	// --------------------------------------------------------
	bool CheckHit();

	// --------------------------------------------------------
	// Set Swimmer's state to a new state
	// --------------------------------------------------------
	void SetSwimmerState(SwimmerState newState);

	// --------------------------------------------------------
	// Set Swimmer's lag seconds
	// --------------------------------------------------------
	void SetLagSeconds(float lagSeconds);

	// --------------------------------------------------------
	// Get the state of the swimmer
	// --------------------------------------------------------
	SwimmerState GetState();
};

