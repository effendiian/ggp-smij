#pragma once
#include <DirectXMath.h>
#include "Entity.h"

//Enum for swimmer states
enum class SwimmerState { Entering, Floating, Joining, Following, Docking };

class Swimmer :
	public Entity
{
private:

	//Follow state vars
	SwimmerState swmrState;
	static const int MAX_FPS = 60;
	GameObject* leader;
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
	// Checks if swimmer is currently a follower.
	// --------------------------------------------------------
	bool IsFollower();

	// --------------------------------------------------------
	// Set Swimmer to follow a game object.
	// --------------------------------------------------------
	void StartFollowing(GameObject* leader);

	// --------------------------------------------------------
	// Set Swimmer to stop following a game object.
	// --------------------------------------------------------
	void StopFollowing();

	// Read-only access of the leader.
	GameObject* const& Leader = leader;
};

