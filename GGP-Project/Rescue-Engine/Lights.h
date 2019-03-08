#pragma once
#include <DirectXMath.h>
#include "GameObject.h"

// --------------------------------------------------------
// A directional light struct definition
//
// Directional Light data to be passed to shaders
// --------------------------------------------------------
struct LightStruct
{
	DirectX::XMFLOAT4 AmbientColor;
	DirectX::XMFLOAT4 DiffuseColor;
	DirectX::XMFLOAT3 Position;
	float dummy; //dummy float because of 16-byte rule with DirectX
	DirectX::XMFLOAT3 Direction;
	float intensity;
};

// --------------------------------------------------------
// A light definition
//
// Base light to be inherited by other classes
// --------------------------------------------------------
class Light : public GameObject
{
protected:
	LightStruct* lightStruct;

public:
	// --------------------------------------------------------
	// Constructor - Set up a light with default values.
	// --------------------------------------------------------
	Light();

	// --------------------------------------------------------
	// Constructor - Set up a light
	//
	// diffuseColor - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	Light(DirectX::XMFLOAT4 diffuseColor, float intensity);

	// --------------------------------------------------------
	// Destructor for when an instance is deleted
	// --------------------------------------------------------
	~Light();

	// --------------------------------------------------------
	// Get the light struct to pass to the shader
	// --------------------------------------------------------
	LightStruct* GetLightStruct();

	// --------------------------------------------------------
	// Set the diffuse color of this light
	// --------------------------------------------------------
	void SetDiffuseColor(DirectX::XMFLOAT4 diffuseColor);

	// --------------------------------------------------------
	// Set the diffuse color of this light
	//
	// r - red
	// g - green
	// b - blue
	// a - alpha
	// --------------------------------------------------------
	void SetDiffuseColor(float r, float g, float b, float a);

	// --------------------------------------------------------
	// Get the diffuse color of this light
	// --------------------------------------------------------
	DirectX::XMFLOAT4 GetDiffuseColor();

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
	// ambientColor - Ambient color of the light
	// diffuseColor - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	DirectionalLight(DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 diffuseColor, float intensity);

	// --------------------------------------------------------
	// Destructor for when an instance is deleted
	// --------------------------------------------------------
	~DirectionalLight();
	
	// --------------------------------------------------------
	// Set the ambient color of this light
	// --------------------------------------------------------
	void SetAmbientColor(DirectX::XMFLOAT4 ambientColor);
	
	// --------------------------------------------------------
	// Set the diffuse color of this light
	//
	// r - red
	// g - green
	// b - blue
	// a - alpha
	// --------------------------------------------------------
	void SetAmbientColor(float r, float g, float b, float a);

	// --------------------------------------------------------
	// Get the ambient color of this light
	// --------------------------------------------------------
	DirectX::XMFLOAT4 GetAmbientColor();

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
private:
	float radius;

public:
	// --------------------------------------------------------
	// Constructor - Set up a point light with default values.
	// --------------------------------------------------------
	PointLight();

	// --------------------------------------------------------
	// Constructor - Set up a point light
	//
	// radius - radius of the point light's sphere
	// diffuseColor - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	PointLight(float radius, DirectX::XMFLOAT4 diffuseColor, float intensity);

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
private:
	float spotRadius;

public:
	// --------------------------------------------------------
	// Constructor - Set up a spot light with default values.
	// --------------------------------------------------------
	SpotLight();

	// --------------------------------------------------------
	// Constructor - Set up a spot light
	//
	// spotRadius - radius of the spot light's "spot"
	// diffuseColor - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	SpotLight(float spotRadius, DirectX::XMFLOAT4 diffuseColor, float intensity);

	// --------------------------------------------------------
	// Destructor for when an instance is deleted
	// --------------------------------------------------------
	~SpotLight();

	// --------------------------------------------------------
	// Set the spot radius of this light
	// --------------------------------------------------------
	void SetSpotRadius(float radius);

	// --------------------------------------------------------
	// Get the spot radius of this light
	// --------------------------------------------------------
	float GetSpotRadius();

	// --------------------------------------------------------
	// Get the direction of this light
	// --------------------------------------------------------
	DirectX::XMFLOAT3 SpotLight::GetDirection();
};