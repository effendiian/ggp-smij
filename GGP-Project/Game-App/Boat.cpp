#include "Boat.h"
#include "ExtendedMath.h"
#include "SwimmerManager.h"
#include "EntityManager.h"

#if defined(DEBUG) || defined(_DEBUG)
#include "ResourceManager.h"
#endif

using namespace std;
using namespace DirectX;

Boat::Boat(Mesh * mesh, Material * material, XMFLOAT2 bounds) : Entity(mesh, material, "player")
{
	state = BoatState::Starting;
	trail = std::vector<Swimmer*>();
	swimmerManager = SwimmerManager::GetInstance();
	inputManager = InputManager::GetInstance();
	levelBounds = bounds;
}

Boat::~Boat()
{ }

// Calls Input, Move, and CheckCollisions every frame
void Boat::Update(float deltaTime)
{
	//Gameover state
	if (state == BoatState::Crashed)
		return;

	switch (state)
	{
	case BoatState::Starting:
		InputStart();
		break;
	
	case BoatState::Playing:
		Input(deltaTime);
		Move(deltaTime);
		CheckCollisions();
		break;
	
	case BoatState::Crashed:
		break;
	
	case BoatState::Resetting:
		SeekOrigin(deltaTime);
		break;

	default:
		break;
	}

}

// Interprets key input for starting the game
void Boat::InputStart()
{
	//Left
	if (inputManager->GetKey(VK_LEFT) || inputManager->GetKey(VK_UP) ||
		inputManager->GetKey('A') || inputManager->GetKey('W') ||
		inputManager->GetKey(VK_RIGHT) || inputManager->GetKey(VK_DOWN) ||
		inputManager->GetKey('D') || inputManager->GetKey('S'))
	{
		state = BoatState::Playing;
	}
}

// Interprets key input
void Boat::Input(float deltaTime)
{
	//Left
	if ((inputManager->GetKey(VK_LEFT) || inputManager->GetKey(VK_UP) ||
		inputManager->GetKey('A') || inputManager->GetKey('W')))
	{
		Rotate(0, turnSpeed * deltaTime * -1, 0);
	}
	//Right
	else if ((inputManager->GetKey(VK_RIGHT) || inputManager->GetKey(VK_DOWN) ||
		inputManager->GetKey('D') || inputManager->GetKey('S')))
	{
		Rotate(0, turnSpeed * deltaTime * 1, 0);
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
		swimmer->SetDebug(true);

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
	ClearSwimmers();

	// Set crashed to false.
	state = BoatState::Resetting;
	seekTimer = 0;
	seekPos = GetPosition();
}

// Moves the boat forward
void Boat::Move(float deltaTime)
{
	MoveRelative(XMFLOAT3(0, 0, speed * deltaTime));
}

// Seeks 0,0,0 
void Boat::SeekOrigin(float deltaTime)
{
	seekTimer += deltaTime;

	//Check when seek is finished
	if (seekTimer >= 1)
	{
		state = BoatState::Starting;
		SetPosition(0, 0, 0);
		return;
	}

	//Lerp movement vector
	XMVECTOR move = XMVectorLerp(XMLoadFloat3(&seekPos), XMVectorSet(0, 0, 0, 0), seekTimer);

	//Calculate arc
	float arc = sin(seekTimer * XM_PI) * 3; //arc height is 2

	//Move the boat
	XMFLOAT3 movement;
	XMStoreFloat3(&movement, move);
	movement.y = arc;
	SetPosition(movement);

	//Rotate the boat
	XMVECTOR slerp = XMQuaternionSlerp(XMLoadFloat4(&GetRotation()), XMVectorSet(0, 0, 0, 1), seekTimer);
	XMFLOAT4 rotation;
	XMStoreFloat4(&rotation, slerp);
	SetRotation(rotation);
}

// Checks for collisions and calls corresponding collide methods
void Boat::CheckCollisions()
{
	float x = this->GetPosition().x;
	float z = this->GetPosition().z;
	//Checking Within Level Bounds
	if (!ExtendedMath::InRange(x, levelBounds.x) ||
		!ExtendedMath::InRange(z, levelBounds.y))
	{
		//Game Over
		GameOver();
		return;
	}
	
	// Check collisions with swimmers in our trail
	for (int i = 1; i < trail.size(); i++)
	{
		Swimmer* swmr = trail[i];
		if (swmr != nullptr	&& swmr->IsFollowing() 
			&& GetCollider()->Collides(*swmr->GetCollider()))
		{
			GameOver();
			return;
		}
	}

	// Check collisions with swimmers floating in the scene
	for (int i = swimmerManager->GetSwimmerCount() - 1; i >= 0; i--) 
	{
		Swimmer* swmr = swimmerManager->GetSwimmer(i);
		if (swmr != nullptr && !swmr->IsFollowing() &&
			GetCollider()->Collides(*swmr->GetCollider()))
		{
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
	this->state = BoatState::Crashed;
	if (trail.size() > 0) { trail[0]->SetSwimmerState(SwimmerState::Hitting); }
}

// Get whether the boat is crashed or not
BoatState Boat::GetState()
{
	return state;
}

// Clears all swimmers from the boat
void Boat::ClearSwimmers()
{
	for (int i = 0; i < trail.size(); i++)
	{
		if (trail[i] != nullptr)
			trail[i]->SetSwimmerState(SwimmerState::Leaving);
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