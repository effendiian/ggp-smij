#pragma once
#include <DirectXMath.h>
#include "Entity.h"

class Swimmer :
	public Entity
{
private:

	//Follow state vars
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

public:
	Swimmer(Mesh* mesh, Material* material);
	Swimmer(Mesh* mesh, Material* material, GameObject* leader);
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

