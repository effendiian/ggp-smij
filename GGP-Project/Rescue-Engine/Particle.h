#pragma once
#include <DirectXMath.h>
#include "Entity.h"

using namespace DirectX;

//Outlines the properties of a single particle in a particle system.
class Particle : public Entity
{
public:

	// Creates a new particle ----------------------------
	Particle(Mesh* mesh, Material* material, std::string name)
		: Entity(mesh, material, name)
	{
		Reset();
	}

	void Reset()
	{
		this->SetParticlePosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
		direction = XMFLOAT3(0.0f, 1.0f, 0.0f);
		force = 10.0f;
		slowdown = .9f;
		gravity   = 1.0f;
	}
	// ---------------------------------------------------

	// Destroys the particle -----------------------------
	virtual ~Particle() {}
	// ---------------------------------------------------

	// Getters and setters -------------------------------
	XMFLOAT3 GetParticlePosition()            { return GetPosition(); }
	void     SetParticlePosition(XMFLOAT3 p)  { this->SetPosition(p); }
	XMFLOAT3 GetParticleDirection()           { return direction;     }
	void     SetParticleDirection(XMFLOAT3 d) { direction = d;        }
	float    GetParticleGravity()             { return gravity;       }
	void     SetParticleGravity(float g)      { gravity = g;          }
	float    GetParticleForce()               { return force;         }
	void     SetParticleForce(float f)        { force = f;            }
	float    GetParticleSlowdown()            { return slowdown;      }
	void     SetParticleSlowdown(float s)     { slowdown = s;         }
	int      GetParticleTimer()               { return timer;         }
	void     SetParticleTimer(int t)          { timer = t;            }
	// ---------------------------------------------------

	//Updates the particle -------------------------------
	void Update(float deltaTime)
	{
		XMFLOAT3 gravityf(0.0f, -abs(gravity), 0.0f); //Create the gravity vector
		XMFLOAT3 POSDUMP;
		XMVECTOR xmp  = XMLoadFloat3(&this->GetPosition());   //Load the position vector
		XMVECTOR xmd  = XMLoadFloat3(&direction);             //Load the direction vector
		XMVECTOR xmg  = XMLoadFloat3(&gravityf);              //Load the gravity vector
		XMVECTOR xmds = XMVectorScale(xmd, force);            //Multiply the direction by the force
		xmp += xmds;                                          //Add the position and the new force together
		xmp += xmg;                                           //Add the gravity to the position vector
		XMStoreFloat3(&POSDUMP, xmp);                         //Store the new position value
		this->SetPosition(POSDUMP);                           //Set the position in the game manager
		force *= slowdown;                        //Scale the force 
	}
	// ---------------------------------------------------

private:

	XMFLOAT3 direction; //The direction the particle is going in
	float gravity;      //The gravity of the particle
	float force;        //The force of the particle
	float slowdown;     //How fast the particle slows down
	float aliveTime;    //How long the particle should stay alive
	int timer;          //How long the particle has been alive

};