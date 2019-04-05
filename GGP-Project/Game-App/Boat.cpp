#include "Boat.h"
#include "ExtendedMath.h"
#include "SwimmerManager.h"
#include "ResourceManager.h"
#include "EntityManager.h"

using namespace std;
using namespace DirectX;

Boat::Boat(Mesh * mesh, Material * material) : Entity(mesh, material, "player")
{
	crashed = false;
	swimmerTrail = std::vector<Swimmer*>();
	swimmerManager = SwimmerManager::GetInstance();
	inputManager = InputManager::GetInstance();
	SetLevelBounds(13.0f, 13.0f);
}

Boat::~Boat()
{ }

// Calls Input, Move, and CheckCollisions every frame
void Boat::Update(float deltaTime)
{
	if (crashed)
	{
		//Check for reset input
		if (inputManager->GetKey(VK_SPACE) && crashed)
		{
			this->Reset();
		}
		return;
	}

	Input(deltaTime);
	Move(deltaTime);
	CheckCollisions();
}

// Interprets key input
void Boat::Input(float deltaTime)
{
	//Left
	if (inputManager->GetKey(VK_LEFT) && !crashed)
	{
		this->Rotate(0, turnSpeed * deltaTime * -1, 0);
	}
	//Right
	else if (inputManager->GetKey(VK_RIGHT) && !crashed)
	{
		this->Rotate(0, turnSpeed * deltaTime * 1, 0);
	}

//If we are in debug, allow us to click to spawn swimmers on our tail
#if defined(DEBUG) || defined(_DEBUG)
	if (inputManager->GetMouseButtonDown(MouseButtons::L))
	{
		Swimmer* debugSwimmer = SwimmerManager::GetInstance()->SpawnSwimmer();
		this->AttachSwimmer(debugSwimmer);
	}
#endif
}

void Boat::Reset() 
{
	// Detach all swimmers.
	this->ClearSwimmers();

	// Reset position.
	this->SetPosition(0, 0, 0);

	// Set crashed to false.
	this->crashed = false;

	// Reset swimmer manager.
	swimmerManager->Reset();
}

// Moves the boat forward
void Boat::Move(float deltaTime)
{
	this->MoveRelative(XMFLOAT3(speed * deltaTime, 0, 0));
}

// Checks for collisions and calls corresponding collide methods
void Boat::CheckCollisions()
{
	float x = this->GetPosition().x;
	float z = this->GetPosition().z;
	//Checking Within Level Bounds
	if (!ExtendedMath::InRange(x, levelWidth) ||
		!ExtendedMath::InRange(z, levelHeight))
	{
		//Game Over
		GameOver();
		return;
	}

		// Check collisions with other entities.
		/*EntityManager* entityManager = EntityManager::GetInstance();
		for (auto i = 0; i < entityManager->EntityCount; i++) 
		{
			Entity* tempRef = entityManager->GetEntity(i);
			if (tempRef != nullptr && tempRef != this && tempRef->Enable)
			{
				if (this->GetCollider()->Collides(*tempRef->GetCollider()))
				{
					printf("Collision with entity.\n");
				}
			}
		}*/
		
	// Check collisions with swimmers.
	for (auto i = 0; i < swimmerManager->SwimmerCount; i++) 
	{
		Swimmer* tempRefSwimmer = swimmerManager->GetSwimmer(i + 1);
		if (tempRefSwimmer != nullptr && tempRefSwimmer->GetEnabled() && !tempRefSwimmer->IsFollower()) 
		{
			if (this->GetCollider()->Collides(*tempRefSwimmer->GetCollider())) 
			{
				printf("Collision with swimmer.\n");
				this->AttachSwimmer(tempRefSwimmer);
			}
		}
	}			
}

// Runs the calls for when the player gets a gameover (hits a wall, etc)
void Boat::GameOver()
{
	ClearSwimmers();
	printf("Game Over! Press the 'Spacebar' to reset.\n");
	this->crashed = true;
	// XMFLOAT3 resetPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	// this->SetPosition(resetPosition);
}

// Attach a swimmer at the end of the trail
Swimmer* Boat::AttachSwimmer(Swimmer* swimmer) 
{
	if (swimmer != nullptr) {
		// Get the swimmer manager reference.
		SwimmerManager* swimmerManager = SwimmerManager::GetInstance();

		// Get the leader.
		GameObject* leader =
			(swimmerTrail.size() < 1)
			? (GameObject*)(this) : (GameObject*)(swimmerTrail[swimmerTrail.size() - 1]);

		// Set the swimmer's position and rotation.
		swimmer->SetPosition(leader->GetPosition());
		swimmer->SetRotation(leader->GetRotation());

		// Attach the swimmer.
		swimmerTrail.push_back(swimmer);
		swimmerManager->AttachSwimmer(swimmer, leader);
	}

	// Return the swimmer.
	return swimmer;
}

//Clears the swimmers from the player
void Boat::ClearSwimmers()
{
	/* for (size_t i = this->swimmerTrail.size() - 1; i > 0; i--)
	{
		//this->swimmers[i]->RemoveFromRenderList();
		//delete[] swimmers[i]; //Not deleting swimmers correctly?
	}*/

	this->DetachSwimmers();
	swimmerTrail.clear();
	swimmerTrail = std::vector<Swimmer*>();
}

// Detach all swimmers.
void Boat::DetachSwimmers() 
{
	for (auto i = 0; i < swimmerTrail.size(); i++)
	{
		if (swimmerTrail[i] != nullptr)
		{
			Swimmer* retiringSwimmer = swimmerTrail[i];
			retiringSwimmer->StopFollowing();
			EntityManager::GetInstance()->RemoveEntity(retiringSwimmer);
			retiringSwimmer->SetEnabled(false);
		}
	}
}