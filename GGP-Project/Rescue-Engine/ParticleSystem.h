#pragma once
#include <vector>
#include <Entity.h>
#include <Particle.h>

using namespace std;

class ParticleSystem
{
public:

	// Declares a "type" of particle system --------------
	enum ParticleSystemType
	{
		Splash
	};
	// ---------------------------------------------------

	// Creates a new particle system ---------------------
	ParticleSystem(ParticleSystemType, XMFLOAT3, Mesh*, Material*, int, int, float);
	// ---------------------------------------------------

	// Deletes the particle system -----------------------
	~ParticleSystem();
	// ---------------------------------------------------

	// Accessory methods ---------------------------------
	bool ClearParticleList();
	void Update(float);
	// ---------------------------------------------------

private:

	XMFLOAT3 position;                   //The position of the particle system
	std::vector<Particle*> particleList; //The list of particles in the system
	int aliveTime;                       //The time that the particles should stay alive
	
};

