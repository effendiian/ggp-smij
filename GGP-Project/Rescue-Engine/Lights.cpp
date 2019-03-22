#include "LightManager.h"

using namespace DirectX;

#pragma region Base Light
// ----------------------------------------------------------------------------
//									BASE LIGHT
// ----------------------------------------------------------------------------

// Constructor - Set up a light with default values.
Light::Light(LightType type)
{
	inLightManager = false;

	lightStruct = new LightStruct();
	lightStruct->Type = (int)type;

	lightStruct->Range = 0;

	lightStruct->Intensity = 1;
	lightStruct->Color = XMFLOAT3(0.5f, 0.5f, 0.5f);

	lightStruct->SpotFalloff = 0;
	lightStruct->Padding = XMFLOAT3();
}

// Constructor - Set up a light
Light::Light(LightType type, XMFLOAT3 color, float intensity)
{
	inLightManager = false;

	lightStruct = new LightStruct();
	lightStruct->Type = (int)type;
	lightStruct->Color = color;
	lightStruct->Intensity = intensity;
}

// Destructor for when an instance is deleted
Light::~Light()
{
	if (lightStruct)
		delete lightStruct;
}

// Get the light struct to pass to the shader
LightStruct* Light::GetLightStruct()
{
	//Set direction
	lightStruct->Direction = GetForwardAxis();

	//Set position
	lightStruct->Position = GetPosition();

	return lightStruct;
}

// Get the type of light
LightType Light::GetType()
{
	return (LightType)(lightStruct->Type);
}

// Set the diffuse color of this light
void Light::SetColor(XMFLOAT3 color)
{
	if (inLightManager)
		SetLightListDirty(LightManager::GetInstance());

	lightStruct->Color = color;
}

// Set the diffuse color of this light
void Light::SetColor(float r, float g, float b)
{
	if (inLightManager)
		SetLightListDirty(LightManager::GetInstance());

	lightStruct->Color = XMFLOAT3(r, g, b);
}

// Get the diffuse color of this light
XMFLOAT3 Light::GetColor()
{
	return lightStruct->Color;
}

// Get the intensity for this light
float Light::GetIntensity()
{
	return lightStruct->Intensity;
}

// Set the intensity for this light
void Light::SetIntensity(float intensity)
{
	if (inLightManager)
		SetLightListDirty(LightManager::GetInstance());

	lightStruct->Intensity = intensity;
}
#pragma endregion


#pragma region Directional Light
// ----------------------------------------------------------------------------
//							DIRECTIONAL LIGHT
// ----------------------------------------------------------------------------

// Constructor - Set up a directional light with default values.
// White ambient and diffuse color.
DirectionalLight::DirectionalLight() : Light::Light(LightType::DirectionalLight)
{ }

// Constructor - Set up a directional light
DirectionalLight::DirectionalLight(XMFLOAT3 color, float intensity) :
	Light::Light(LightType::DirectionalLight, color, intensity)
{ }

// Destructor for when an instance is deleted
DirectionalLight::~DirectionalLight()
{ }

// Get the direction of this light
XMFLOAT3 DirectionalLight::GetDirection()
{
	return GetForwardAxis();
}
#pragma endregion


#pragma region Point Light
// ----------------------------------------------------------------------------
//									POINT LIGHT
// ----------------------------------------------------------------------------

// Constructor - Set up a point light with default values.
PointLight::PointLight() : Light::Light(LightType::PointLight)
{ 
	lightStruct->Range = 5;
}

// Constructor - Set up a point light
PointLight::PointLight(float radius, XMFLOAT3 color, float intensity) :
	Light::Light(LightType::PointLight, color, intensity)
{ 
	lightStruct->Range = radius;
}

// Destructor for when an instance is deleted
PointLight::~PointLight()
{ }

// Set the radius of this light
void PointLight::SetRadius(float radius)
{
	if (inLightManager)
		SetLightListDirty(LightManager::GetInstance());

	lightStruct->Range = radius;
}

// Get the radius of this light
float PointLight::GetRadius()
{
	return lightStruct->Range;
}
#pragma endregion


#pragma region Spot Light
// ----------------------------------------------------------------------------
//									 SPOT LIGHT
// ----------------------------------------------------------------------------

// Constructor - Set up a spot light with default values.
SpotLight::SpotLight() : Light::Light(LightType::SpotLight)
{
	lightStruct->SpotFalloff = 5;
	lightStruct->Range = 5;
}

// Constructor - Set up a spot light
SpotLight::SpotLight(float range, float spotFalloff, XMFLOAT3 color, float intensity) :
	Light::Light(LightType::SpotLight, color, intensity)
{
	lightStruct->SpotFalloff = spotFalloff;
	lightStruct->Range = range;
}

// Destructor for when an instance is deleted
SpotLight::~SpotLight()
{ }

// Set the falloff of this spotlight
void SpotLight::SetSpotFalloff(float spotFallOff)
{
	if (inLightManager)
		SetLightListDirty(LightManager::GetInstance());

	lightStruct->SpotFalloff = spotFallOff;
}

// Get the spot falloff of this light
float SpotLight::GetSpotFalloff()
{
	return lightStruct->SpotFalloff;
}

// Set the spot radius of this light
void SpotLight::SetRange(float spotRadius)
{
	if (inLightManager)
		SetLightListDirty(LightManager::GetInstance());

	lightStruct->Range = spotRadius;
}

// Get the spot radius of this light
float SpotLight::GetRange()
{
	return lightStruct->SpotFalloff;
}

// Get the direction of this light
XMFLOAT3 SpotLight::GetDirection()
{
	return GetForwardAxis();
}
#pragma endregion

//Set the light manager's light list to dirty
// THIS FRIEND FUNCTION CAN ONLY BE ACCESSED BY THE LIGHT
//		IN Lights.cpp
void SetLightListDirty(LightManager* lightManager) {
	lightManager->listDirty = true;
}