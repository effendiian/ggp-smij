#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Swimmer.h"
#include "InputManager.h"

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
	InputManager* inputManager;

	//Swimmers
	std::vector<Swimmer*> swimmers;
	const char* swimmerMesh = "Assets\\Models\\sphere.obj";
	const char* swimmerMat = "wood";

public:
	Boat(Mesh* mesh, Material* material);
	~Boat();

	// --------------------------------------------------------
	// Calls Input, Move, and CheckCollisions every frame
	// --------------------------------------------------------
	void Update(float deltaTime);

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
	// Create a swimmer at the end of the trail
	// --------------------------------------------------------
	void CreateSwimmer();

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

};

