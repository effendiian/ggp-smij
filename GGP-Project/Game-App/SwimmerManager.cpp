#include "SwimmerManager.h"
#include "ResourceManager.h"
#include "EntityManager.h"

// Default constructor.
SwimmerManager::SwimmerManager() 
{
	// Seed the random.
	std::random_device rseed;
	rng = std::mt19937(rseed());

	swimmerCount = 0;
	maxSwimmerCount = 10;
	currentTTS = 0;
	this->Reset();
	this->AddCollider(DirectX::XMFLOAT3(10, 0, 10), DirectX::XMFLOAT3(0, 0, 0));
}

// Remove swimmers.
SwimmerManager::~SwimmerManager()
{
	// Reset the manager.
	Reset();
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
		-5, 
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
		(this->enabled) // ...if this object is enabled,
		&& (swimmerCount < maxSwimmerCount) // ...if there is still space,
		&& (currentTTS >= maxTTS) // ...and if the timer surpassed the delay amount.
	); 
}

// Reset the manager.
void SwimmerManager::Reset() 
{
	EntityManager* entityManager = EntityManager::GetInstance();
	for (int i = 0; i < swimmers.size(); i++)
	{
		if(swimmers[i] != nullptr)
			entityManager->RemoveEntity(swimmers[i]);
	}
	swimmers.clear();
	swimmerCount = 0;
	currentTTS = 0;
}

// Increase the level.
void SwimmerManager::IncreaseLevel() 
{
	maxSwimmerCount += 5;
}

// Update swimmer state and spawn time.
void SwimmerManager::Update(float deltaTime) 
{
	// Check if the manager is enabled.
	if (this->enabled) {

		// Update the time till spawn.
		currentTTS += deltaTime;

		// Check if it's ready to spawn a new swimmer.
		if (IsReadyToSpawn()) 
		{
			// Spawn a new swimmer, if time permits.
			SpawnSwimmer();
			currentTTS = 0;
		}
	}
}

// Create a swimmer and spawn at a random position.
Swimmer* SwimmerManager::SpawnSwimmer()
{
		// To be refactored into the swimmer manager.
		ResourceManager* resourceManager = ResourceManager::GetInstance();

		// Create the swimmer name.
		std::string swimmer_id = "swimmer" + std::to_string(swimmerCount + 1);

		// Create the swimmer.
		Swimmer* swimmer = new Swimmer(
			resourceManager->GetMesh(swimmerMesh),
			resourceManager->GetMaterial(swimmerMat),
			swimmer_id
		);

		// Add collider.
		swimmer->AddCollider(DirectX::XMFLOAT3(0.9f, 0.9f, 0.9f), DirectX::XMFLOAT3(0, 0, 0));
#if defined(DEBUG) || defined(_DEBUG)
		swimmer->GetCollider()->SetDebug(true);
#endif

		// Instantiate the position and rotation.
		this->RandomizeSwimmer(swimmer);

		// Return the swimmer.
		swimmers.push_back(swimmer);
		return swimmer;
}

// Retrieve swimmer.
Swimmer* SwimmerManager::GetSwimmer(int id) 
{
	if (id < 0 || id >= swimmers.size()) { return nullptr; }
	return swimmers[id];
}

// Get swimmer count
int SwimmerManager::GetSwimmerCount()
{
	return (int)swimmers.size();
}

// Attach swimmer to the input object.
void SwimmerManager::AttachSwimmer(Swimmer* swimmer, Entity* leader, int index)
{
	swimmer->JoinTrail(leader);
	
	//Remove from the list of floating swimmers
	//Swap it for the last one
	std::swap(swimmers[index], swimmers[swimmers.size() - 1]);

	//Pop the last one
	swimmers.pop_back();
}
