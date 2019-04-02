#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Entity.h"

class Boat :
	public Entity
{
private:
	float speed = 5;
	float turnSpeed = 100;
	float minDistance = 0.5f;

	bool crashed;
	std::vector<GameObject*> nodes;
	std::vector<DirectX::XMFLOAT3> path;
	float timer;
public:
	Boat(Mesh* mesh, Material* material, DirectX::XMFLOAT3 position);
	~Boat();

	//methods

	// --------------------------------------------------------
	// Calls Input, Move, and CheckCollisions every frame
	// --------------------------------------------------------
	void Update();

	// --------------------------------------------------------
	// Interprets key input
	// --------------------------------------------------------
	void Input();

	// --------------------------------------------------------
	// Moves the boat forward
	// --------------------------------------------------------
	void Move();

	// --------------------------------------------------------
	// Checks for collisions and calls corresponding collide methods
	// --------------------------------------------------------
	void CheckCollisions();

};

