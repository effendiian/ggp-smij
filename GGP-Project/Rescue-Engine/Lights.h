#pragma once
#include <DirectXMath.h>
#include "GameObject.h"

enum class LightType { DirectionalLight = 0, PointLight = 1, SpotLight = 2};

#define MAX_LIGHTS 10

// --------------------------------------------------------
// A light struct definition
//
// Light data to be passed to shaders
// --------------------------------------------------------
struct LightStruct
{
	int					Type;
	DirectX::XMFLOAT3	Direction;	// 16 bytes

	float				Range;
	DirectX::XMFLOAT3	Position;	// 32 bytes

	float				Intensity;
	DirectX::XMFLOAT3	Color;		// 48 bytes

	float				SpotFalloff;
	DirectX::XMFLOAT3	Padding;	// 64 bytes
};

// --------------------------------------------------------
// An ambient light struct definition
//
// Ambient light data to be passed to shaders
// --------------------------------------------------------
struct AmbientLightStruct
{
	float				Intensity;
	DirectX::XMFLOAT3	Color;		// 48 bytes
};

// --------------------------------------------------------
// A light definition
//
// Base light to be inherited by other classes
// --------------------------------------------------------
class Light : public GameObject
{
private:

	// --------------------------------------------------------
	// Set whether this light is in the light manager.
	// THIS FUNCTION CAN ONLY BE ACCESSED BY THE LIGHT MANAGER
	//		IN LightManager.cpp
	// --------------------------------------------------------
	friend void SetInLightManager(Light* light, bool val);

protected:
	bool inLightManager;
	LightStruct* lightStruct;

public:

	// --------------------------------------------------------
	// Constructor - Set up a light with default values.
	//
	// type - the type of light this is
	// --------------------------------------------------------
	Light(LightType type);

	// --------------------------------------------------------
	// Constructor - Set up a light
	//
	// type - the type of light this is
	// color - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	Light(LightType type, DirectX::XMFLOAT3 color, float intensity);

	// --------------------------------------------------------
	// Destructor for when an instance is deleted
	// --------------------------------------------------------
	~Light();

	// --------------------------------------------------------
	// Get the light struct to pass to the shader
	// --------------------------------------------------------
	LightStruct* GetLightStruct();

	// --------------------------------------------------------
	// Get the type of light
	// --------------------------------------------------------
	LightType GetType();

	// --------------------------------------------------------
	// Set the diffuse color of this light
	// --------------------------------------------------------
	void SetColor(DirectX::XMFLOAT3 color);

	// --------------------------------------------------------
	// Set the diffuse color of this light
	//
	// r - red
	// g - green
	// b - blue
	// --------------------------------------------------------
	void SetColor(float r, float g, float b);

	// --------------------------------------------------------
	// Get the diffuse color of this light
	// --------------------------------------------------------
	DirectX::XMFLOAT3 GetColor();

	// --------------------------------------------------------
	// Get the intensity for this light
	// --------------------------------------------------------
	float GetIntensity();

	// --------------------------------------------------------
	// Set the intensity for this light
	// --------------------------------------------------------
	void SetIntensity(float intensity);
};

// --------------------------------------------------------
// A directional light definition
//
// Directional Lights are applied to all objects
// --------------------------------------------------------
class DirectionalLight : public Light
{
public:
	// --------------------------------------------------------
	// Constructor - Set up a directional light with default values.
	// --------------------------------------------------------
	DirectionalLight();

	// --------------------------------------------------------
	// Constructor - Set up a directional light
	//
	// color - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	DirectionalLight(DirectX::XMFLOAT3 color, float intensity);

	// --------------------------------------------------------
	// Destructor for when an instance is deleted
	// --------------------------------------------------------
	~DirectionalLight();

	// --------------------------------------------------------
	// Get the direction of this light
	// --------------------------------------------------------
	DirectX::XMFLOAT3 GetDirection();
};

// --------------------------------------------------------
// A point light definition
//
// Point Lights are applied to all in a sphere
// --------------------------------------------------------
class PointLight : public Light
{
public:
	// --------------------------------------------------------
	// Constructor - Set up a point light with default values.
	// --------------------------------------------------------
	PointLight();

	// --------------------------------------------------------
	// Constructor - Set up a point light
	//
	// radius - radius of the point light's sphere
	// color - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	PointLight(float radius, DirectX::XMFLOAT3 color, float intensity);

	// --------------------------------------------------------
	// Destructor for when an instance is deleted
	// --------------------------------------------------------
	~PointLight();

	// --------------------------------------------------------
	// Set the radius of this light
	// --------------------------------------------------------
	void SetRadius(float radius);

	// --------------------------------------------------------
	// Get the radius of this light
	// --------------------------------------------------------
	float GetRadius();

};

// --------------------------------------------------------
// A spot light definition
//
// Point Lights are applied to all in a sphere
// --------------------------------------------------------
class SpotLight : public Light
{
public:
	// --------------------------------------------------------
	// Constructor - Set up a spot light with default values.
	// --------------------------------------------------------
	SpotLight();

	// --------------------------------------------------------
	// Constructor - Set up a spot light
	//
	// range - the range of this spotlight
	// spotFallOff - the falloff for this spotlight
	// color - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	SpotLight(float range, float spotFalloff, DirectX::XMFLOAT3 color, float intensity);

	// --------------------------------------------------------
	// Destructor for when an instance is deleted
	// --------------------------------------------------------
	~SpotLight();

	// --------------------------------------------------------
	// Set the falloff of this spotlight
	// --------------------------------------------------------
	void SetSpotFalloff(float spotFallOff);

	// --------------------------------------------------------
	// Get the spot falloff of this light
	// --------------------------------------------------------
	float GetSpotFalloff();

	// --------------------------------------------------------
	// Set the spot radius of this light
	// --------------------------------------------------------
	void SetRange(float radius);

	// --------------------------------------------------------
	// Get the spot radius of this light
	// --------------------------------------------------------
	float GetRange();

	// --------------------------------------------------------
	// Get the direction of this light
	// --------------------------------------------------------
	DirectX::XMFLOAT3 SpotLight::GetDirection();
};