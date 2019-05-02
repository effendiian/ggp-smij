#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Entity.h"
#include "Swimmer.h"
#include "InputManager.h"
#include "SwimmerManager.h"

enum class BoatState { Starting, Playing, Crashed, Resetting };

class Boat :
	public Entity
{
private:

	//Movement
	float speed = 3;
	float turnSpeed = 150;
	float minDistance = 0.5f;
	float levelRadius;
	BoatState state;
	SwimmerManager* swimmerManager;
	InputManager* inputManager;
	std::vector<Swimmer*> trail;

	//Seek timer
	float seekTimer;
	DirectX::XMFLOAT3 seekPos;

	// --------------------------------------------------------
	// Attach a swimmer at the end of the trail
	// --------------------------------------------------------
	void AttachSwimmer(Swimmer* swimmer, int index);
	
public:
	Boat(Mesh* mesh, Material* material, float levelRadius);
	~Boat();

	// --------------------------------------------------------
	// Calls Input, Move, and CheckCollisions every frame
	// --------------------------------------------------------
	void Update(float deltaTime);

	// --------------------------------------------------------
	// reset position and status of boat.
	// --------------------------------------------------------
	void Reset();

	// --------------------------------------------------------
	// Interprets key input for starting the game
	// --------------------------------------------------------
	void InputStart();

	// --------------------------------------------------------
	// Interprets key input
	// --------------------------------------------------------
	void Input(float deltaTime);

	// --------------------------------------------------------
	// Moves the boat forward
	// --------------------------------------------------------
	void Move(float deltaTime);

	// --------------------------------------------------------
	// Seeks 0,0,0 
	// --------------------------------------------------------
	void SeekOrigin(float deltaTime);

	// --------------------------------------------------------
	// Checks for collisions and calls corresponding collide methods
	// --------------------------------------------------------
	void CheckCollisions();

	// --------------------------------------------------------
	// Code called when the player hits the edge of the level
	// --------------------------------------------------------
	void GameOver();

	// --------------------------------------------------------
	// Get whether the boat is crashed or not
	// --------------------------------------------------------
	BoatState GetState();
	
	// --------------------------------------------------------
	// Clears all swimmers from the boat
	// --------------------------------------------------------
	void ClearSwimmers();
};
