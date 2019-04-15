#include "Boat.h"
#include "ExtendedMath.h"
#include "SwimmerManager.h"
#include "EntityManager.h"

#if defined(DEBUG) || defined(_DEBUG)
#include "ResourceManager.h"
#endif

using namespace std;
using namespace DirectX;

Boat::Boat(Mesh * mesh, Material * material) : Entity(mesh, material, "player")
{
	crashed = false;
	trail = std::vector<Swimmer*>();
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
		// Create the swimmer.
		Swimmer* swimmer = new Swimmer(
			ResourceManager::GetInstance()->GetMesh("Assets\\Models\\sphere.obj"),
			ResourceManager::GetInstance()->GetMaterial("wood"),
			"swimmer"
		);
		swimmer->AddCollider(DirectX::XMFLOAT3(0.9f, 0.9f, 0.9f), DirectX::XMFLOAT3(0, 0, 0));
		swimmer->GetCollider()->SetDebug(true);

		// Get the leader.
		Entity* leader = nullptr;
		if (trail.size() < 1)
			leader = this;
		else leader = trail[trail.size() - 1];
		swimmer->JoinTrail(leader);

		trail.push_back(swimmer);
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
		printf("Collision with boundary.\n");
		GameOver();
		return;
	}
	
	// Check collisions with swimmers in our trail
	for (int i = 0; i < trail.size(); i++)
	{
		Swimmer* swmr = trail[i];
		if (swmr != nullptr && swmr->IsFollowing() &&
			GetCollider()->Collides(*swmr->GetCollider()))
		{
			//printf("Collision with swimmer.\n");
			//TODO: Add implementation of swimmers joining the trail
			//GameOver();
			//return;
		}
	}

	// Check collisions with swimmers floating in the scene
	for (int i = swimmerManager->GetSwimmerCount() - 1; i >= 0; i--) 
	{
		Swimmer* swmr = swimmerManager->GetSwimmer(i);
		if (swmr != nullptr && !swmr->IsFollowing() &&
			GetCollider()->Collides(*swmr->GetCollider()))
		{
			printf("Collision with swimmer.\n");
			AttachSwimmer(swmr, i);
		}
	}
}

// Runs the calls for when the player gets a gameover (hits a wall, etc)
void Boat::GameOver()
{
	for (int i = 1; i < trail.size(); i++)
	{
		if (trail[i]) 
		{
			trail[i]->SetSwimmerState(SwimmerState::Still);
		}
	}

	printf("Game Over! Press the 'Spacebar' to reset.\n");
	this->crashed = true;
	if (trail.size() > 0) { trail[0]->SetSwimmerState(SwimmerState::Hitting); }
}

// Clears all swimmers from the boat
void Boat::ClearSwimmers()
{
	for (int i = 0; i < trail.size(); i++)
	{
		if (trail[i] != nullptr)
			EntityManager::GetInstance()->RemoveEntity(trail[i]);
	}
	trail.clear();
}

// Attach a swimmer at the end of the trail
void Boat::AttachSwimmer(Swimmer* swimmer, int index) 
{
	// Get the leader.
	Entity* leader = nullptr;
	if (trail.size() < 1)
		leader = this;
	else leader = trail[trail.size() - 1];
		
	// Attach the swimmer.
	trail.push_back(swimmer);
	swimmerManager->AttachSwimmer(swimmer, leader, index);
}