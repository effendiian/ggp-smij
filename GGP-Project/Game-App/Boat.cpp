#include "Boat.h"
#include "SwimmerManager.h"

using namespace std;
using namespace DirectX;

Boat::Boat(Mesh * mesh, Material * material) : Entity(mesh, material)
{
	crashed = false;
	swimmerTrail = std::vector<Swimmer*>();
	swimmerManager = SwimmerManager::GetInstance();
	inputManager = InputManager::GetInstance();
}

Boat::~Boat()
{ }

// Calls Input, Move, and CheckCollisions every frame
void Boat::Update(float deltaTime)
{
	if (this->Enable) 
	{
		Input(deltaTime);
	}

	if (!crashed) 
	{
		Move(deltaTime);
		CheckCollisions();
	}
}

// Interprets key input
void Boat::Input(float deltaTime)
{
	//Reset
	if (inputManager->GetKey(VK_SPACE)) 
	{
		this->Reset();
	}

	//Left
	if (inputManager->GetKey(VK_LEFT))
	{
		this->Rotate(0, turnSpeed * deltaTime * -1, 0);
	}
	//Right
	else if (inputManager->GetKey(VK_RIGHT))
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
	this->DetachSwimmers();

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
}

// Attach a swimmer at the end of the trail
Swimmer* Boat::AttachSwimmer(Swimmer* swimmer) 
{
	if (swimmer != nullptr) {
		// Get the swimmer manager reference.
		SwimmerManager* swimmerManager = SwimmerManager::GetInstance();

		// Get the leader.
		GameObject* leader =
			(swimmerManager->SwimmerCount < 1)
			? (GameObject*)(this) : (GameObject*)(swimmerManager->GetSwimmer(swimmerManager->SwimmerCount));

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

// Detach a swimmer found in the trail. Returns nullptr if not in trail already.
Swimmer* Boat::DetachSwimmer(Swimmer* swimmer) 
{
	if (swimmer != nullptr) 
	{
		for (auto i = 0; i < swimmerTrail.size(); i++) 
		{
			if (swimmerTrail[i] != nullptr && swimmerTrail[i] == swimmer)
			{
				// If we're in the middle of the trail, we need to reassign swimmers.
				if (i < swimmerTrail.size() && i > 0) 
				{
					swimmerManager->AttachSwimmer(swimmerTrail[i + 1], swimmerTrail[i - 1]);				
				}

				swimmerManager->DetachSwimmer(swimmer);
				return swimmer;
			}
		}

		return nullptr;
	}
	else 
	{
		return nullptr;
	}
}

// Detach all swimmers.
void Boat::DetachSwimmers() 
{
	for (auto i = 0; i < swimmerTrail.size(); i++)
	{
		if (swimmerTrail[i] != nullptr)
		{
			swimmerManager->DetachSwimmer(swimmerTrail[i]);
		}
	}
}