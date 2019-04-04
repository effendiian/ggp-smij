#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Entity.h"
#include "Swimmer.h"
#include "InputManager.h"
#include "SwimmerManager.h"

class Boat :
	public Entity
{
private:

	//Movement
	float speed = 2;
	float turnSpeed = 100;
	float minDistance = 0.5f;
	float levelWidth;
	float levelHeight;
	bool crashed;
	SwimmerManager* swimmerManager;
	InputManager* inputManager;
	std::vector<Swimmer*> swimmerTrail;
	
public:
	Boat(Mesh* mesh, Material* material);
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
	// Interprets key input
	// --------------------------------------------------------
	void Input(float deltaTime);

	// --------------------------------------------------------
	// Moves the boat forward
	// --------------------------------------------------------
	void Move(float deltaTime);

	// --------------------------------------------------------
	// Checks for collisions and calls corresponding collide methods
	// --------------------------------------------------------
	void CheckCollisions();

	// --------------------------------------------------------
	// Code called when the player hits the edge of the level
	// --------------------------------------------------------
	void GameOver();
	
	// --------------------------------------------------------
	// Clears all swimmers from the boat
	// --------------------------------------------------------
	void ClearSwimmers();

	//GETTERS & SETTERS
	void SetLevelWidth (float value)               { if (value > 0) levelWidth  = value;           }
	void SetLevelHeight(float value)               { if (value > 0) levelHeight = value;           }
	void SetLevelBounds(float width, float height) { SetLevelWidth(width); SetLevelHeight(height); }
	float GetLevelWidth () { return levelWidth;  }
	float GetLevelHeight() { return levelHeight; }

	// Attach a swimmer at the end of the trail
	// --------------------------------------------------------
	Swimmer* AttachSwimmer(Swimmer*);
	
	// --------------------------------------------------------
	// Clear the trail.
	// --------------------------------------------------------
	void DetachSwimmers();

};
