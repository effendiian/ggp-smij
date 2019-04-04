#include "SwimmerManager.h"
#include "ResourceManager.h"
#include "EntityManager.h"

// Default constructor.
SwimmerManager::SwimmerManager() 
	: SwimmerManager(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 0, 10), DirectX::XMFLOAT3(0, 0, 0)) 
{}

// Collider included constructor.
SwimmerManager::SwimmerManager(DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset)
	: GameObject() 
{
	// Seed the random.
	std::random_device rseed;
	rng = std::mt19937(rseed());

	swimmerCount = 0;
	maxSwimmerCount = 10;
	currentTTS = 0;
	this->Reset();
	this->AddCollider(size, offset);
}

// Position instantiation.
SwimmerManager::SwimmerManager(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset)
	: SwimmerManager(size, offset)
{
	this->SetPosition(position);
}

// Remove swimmers.
SwimmerManager::~SwimmerManager()
{
	// Reset the manager.
	this->Reset();
}

// Get next random position.
DirectX::XMFLOAT3 SwimmerManager::GetNextPosition()
{
	Collider* c = this->GetCollider();

	// Create uniform distribution ranges.
	DirectX::XMFLOAT3 halfSize = c->GetHalfSize();
	std::uniform_real<float> distX(-halfSize.x, halfSize.x);
	std::uniform_real<float> distZ(-halfSize.z, halfSize.z);

	// Create the center position.
	DirectX::XMFLOAT3 center = c->GetPosition();

	// Return next randomly generated position, centered on the swimmer manager's collider.
	return DirectX::XMFLOAT3(
		center.x + distX(rng), 
		center.y, 
		center.z + distZ(rng)
	);
}

// Randomize the swimmer spawn configuration.
void SwimmerManager::RandomizeSwimmer(Swimmer* swimmer) 
{
	if (swimmer != nullptr) 
	{
		swimmer->SetPosition(this->GetNextPosition());
		// swimmer->SetRotation(this->GetNextRotation());
		// swimmer->SetScale(this->GetNextScale());
	}
}

// Check if another swimmer can spawn.
bool SwimmerManager::IsReadyToSpawn()
{
	return ( // Return true...
		(this->Enable) // ...if this object is enabled,
		&& (this->SwimmerCount < this->MaxSwimmerCount) // ...if there is still space,
		&& (currentTTS >= TTS) // ...and if the timer surpassed the delay amount.
	); 
}

// Reset the manager.
void SwimmerManager::Reset() 
{
	if (this->SwimmerCount > 0)
	{
		EntityManager* entityManager = EntityManager::GetInstance();
		for (auto i = this->SwimmerCount; i > 0; i--)
		{
			std::string swimmer_id = "swimmer" + std::to_string(i);
			entityManager->RemoveEntity(swimmer_id);
			swimmerCount--;
		}
	}
	swimmerCount = 0;
	currentTTS = 0;
}

// Increase the level.
void SwimmerManager::IncreaseLevel() 
{
	this->maxSwimmerCount += 5;
}

// Update swimmer state and spawn time.
void SwimmerManager::Update(float deltaTime) 
{
	// Check if the manager is enabled.
	if (this->Enable) {

		// Update the time till spawn.
		this->currentTTS += deltaTime;

		// Check if it's ready to spawn a new swimmer.
		if (IsReadyToSpawn()) 
		{
			// Spawn a new swimmer, if time permits.
			this->SpawnSwimmer();
			this->currentTTS = 0;
		}

		// Update the swimmers specifically.
		for (auto i = 0; i < this->SwimmerCount; i++) 
		{
			// Update the swimmers specifically.
			Swimmer* temp = this->GetSwimmer(i + 1);
			if (temp != nullptr) { temp->Update(deltaTime); }
		}

	}
}

// Create a swimmer and spawn at a random position.
Swimmer* SwimmerManager::SpawnSwimmer()
{
		// To be refactored into the swimmer manager.
		EntityManager* entityManager = EntityManager::GetInstance();
		ResourceManager* resourceManager = ResourceManager::GetInstance();

		// Create the swimmer name.
		std::string swimmer_id = "swimmer" + std::to_string(swimmerCount + 1);

		// Create the swimmer.
		Swimmer* swimmer = new Swimmer(
			resourceManager->GetMesh(swimmerMesh),
			resourceManager->GetMaterial(swimmerMat)
		);

		// Add collider.
		swimmer->AddCollider(DirectX::XMFLOAT3(0.9f, 0.9f, 0.9f), DirectX::XMFLOAT3(0, 0, 0));

		// Instantiate the position and rotation.
		this->RandomizeSwimmer(swimmer);

		// Add entity to the entity manager.
		entityManager->AddEntity(swimmer, swimmer_id);
		this->swimmerCount++; // Increment count after successful addition.

		// Return the swimmer.
		return swimmer;
}

// Retrieve swimmer.
Swimmer* SwimmerManager::GetSwimmer(int id) 
{
	if (id <= 0 || id > this->SwimmerCount) { return nullptr; }
	return (Swimmer*)(EntityManager::GetInstance()->GetEntity("swimmer" + std::to_string(id)));
}

// Attach swimmer to the input object.
void SwimmerManager::AttachSwimmer(Swimmer* swimmer, GameObject* object) 
{
	if (swimmer != nullptr) 
	{
		swimmer->StartFollowing(object);
	}
}

// Detach swimmer from all objects.
void SwimmerManager::DetachSwimmer(Swimmer* swimmer) 
{
	if (swimmer != nullptr) 
	{
		swimmer->StopFollowing();
	}
}