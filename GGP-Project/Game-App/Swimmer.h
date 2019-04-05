#pragma once
#include <DirectXMath.h>
#include "Entity.h"

//Enum for swimmer states
enum class SwimmerState { Entering, Floating, Joining, Following, Docking, Hitting };

class Swimmer :
	public Entity
{
private:

	//Follow state vars
	SwimmerState swmrState;
	static const int MAX_FPS = 60;
	Entity* leader;
	float lagSeconds = 0.7f;

	//Buffer vars
	DirectX::XMFLOAT3* positionBuffer;
	float* timeBuffer;
	int bufferLength;
	int oldestIndex;
	int newestIndex;
	float timer;

	// --------------------------------------------------------
	// Run this swimmer's floating behaviour
	//---------------------------------------------------------
	void Float(float deltaTime);

	// --------------------------------------------------------
	// Run this swimmer's following behaviour
	// --------------------------------------------------------
	void Follow(float deltaTime);

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
	// Set Swimmer's state for when the Boat hits something
	// --------------------------------------------------------
	void StartHit();

	// --------------------------------------------------------
	// Set Swimmer's state for when the Boat is docking the swimmers
	// --------------------------------------------------------
	void StartDock();
};

