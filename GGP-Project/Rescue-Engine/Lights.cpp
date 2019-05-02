#include "LightManager.h"

using namespace DirectX;

#pragma region Base Light
// ----------------------------------------------------------------------------
//									BASE LIGHT
// ----------------------------------------------------------------------------

// Constructor - Set up a light with default values.
Light::Light(LightType type, bool castShadows)
{
	inLightManager = false;
	SetCastsShadows(castShadows);
	shadowDSV = nullptr;
	shadowSRV = nullptr;

	lightStruct = new LightStruct();
	lightStruct->Type = (int)type;

	lightStruct->Range = 0;

	lightStruct->Intensity = 1;
	lightStruct->Color = XMFLOAT3(0.5f, 0.5f, 0.5f);

	lightStruct->SpotFalloff = 0;
	lightStruct->Padding = XMFLOAT3();
}

// Constructor - Set up a light
Light::Light(LightType type, bool castShadows, XMFLOAT3 color, float intensity)
{
	inLightManager = false;
	SetCastsShadows(castShadows);
	shadowDSV = nullptr;
	shadowSRV = nullptr;

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

	if (shadowDSV != nullptr)
		shadowDSV->Release();

	if (shadowSRV != nullptr)
		shadowSRV->Release();
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

// Get whether this light casts shadows or not
bool Light::GetCastsShadows()
{
	return castsShadows;
}

// Set whether this light casts shadows or not
void Light::SetCastsShadows(bool castShadows)
{
	castsShadows = castShadows;
}

// Get the shadow depth/stencil for this light
ID3D11DepthStencilView * Light::GetShadowDSV()
{
	return shadowDSV;
}

// Get the shadow SRV for this light
ID3D11ShaderResourceView* Light::GetShadowSRV()
{
	return shadowSRV;
}

// Create the SRV for this light's shadow map
void Light::InitShadowMap(ID3D11Device* device)
{
	if (shadowSRV != nullptr)
		return;

	//Create the shadow texture
	D3D11_TEXTURE2D_DESC shadowTexDesc = *(LightManager::GetInstance()->GetShadowTexDesc());
	ID3D11Texture2D* shadowTexture;
	device->CreateTexture2D(&shadowTexDesc, 0, &shadowTexture);

	// Create the depth/stencil
	D3D11_DEPTH_STENCIL_VIEW_DESC shadowDSDesc = *(LightManager::GetInstance()->GetShadowDSDesc());
	device->CreateDepthStencilView(shadowTexture, &shadowDSDesc, &shadowDSV);

	// Create the SRV for the shadow map
	D3D11_SHADER_RESOURCE_VIEW_DESC shadowSRVDesc = *(LightManager::GetInstance()->GetShadowSRVDesc());
	device->CreateShaderResourceView(shadowTexture, &shadowSRVDesc, &shadowSRV);

	// Release the texture reference since we don't need it
	shadowTexture->Release();
}

#pragma endregion


#pragma region Directional Light
// ----------------------------------------------------------------------------
//							DIRECTIONAL LIGHT
// ----------------------------------------------------------------------------

// Constructor - Set up a directional light with default values.
// White ambient and diffuse color.
DirectionalLight::DirectionalLight(bool castShadows) : Light::Light(LightType::DirectionalLight, castShadows)
{ 
	CalculateViewMatrix();
	CalculateProjMatrix();
}

// Constructor - Set up a directional light
DirectionalLight::DirectionalLight(bool castShadows, XMFLOAT3 color, float intensity) :
	Light::Light(LightType::DirectionalLight, castShadows, color, intensity)
{ 
	CalculateViewMatrix();
	CalculateProjMatrix();
}

// Destructor for when an instance is deleted
DirectionalLight::~DirectionalLight()
{ }

// Get the direction of this light
XMFLOAT3 DirectionalLight::GetDirection()
{
	return GetForwardAxis();
}

// Calculate view for shadow rendering
void DirectionalLight::CalculateViewMatrix()
{
	XMMATRIX view = XMMatrixTranspose(XMMatrixLookToLH(
		XMVectorSubtract(XMLoadFloat3(&GetPosition()), XMVectorScale(XMLoadFloat3(&GetForwardAxis()), 50)),
		XMLoadFloat3(&GetForwardAxis()),
		XMLoadFloat3(&GetUpAxis())));
	XMStoreFloat4x4(&shadowView, view);
}

// Calculate projection for shadow rendering
void DirectionalLight::CalculateProjMatrix()
{
	XMMATRIX proj = XMMatrixTranspose(XMMatrixOrthographicLH(
		30,
		30,
		0.1f,
		100));
	XMStoreFloat4x4(&shadowProj, proj);
}

// Get this light's view matrix (for shadows)
DirectX::XMFLOAT4X4 DirectionalLight::GetViewMatrix()
{
	//TODO: Calculate matrices only when light changes
	CalculateViewMatrix();
	return shadowView;
}

// Get this light's projection matrix (for shadows)
DirectX::XMFLOAT4X4 DirectionalLight::GetProjectionMatrix()
{
	//TODO: Calculate matrices only when light changes
	CalculateProjMatrix();
	return shadowProj;
}
#pragma endregion


#pragma region Point Light
// ----------------------------------------------------------------------------
//									POINT LIGHT
// ----------------------------------------------------------------------------

// Constructor - Set up a point light with default values.
PointLight::PointLight(bool castShadows) : Light::Light(LightType::PointLight, castShadows)
{ 
	lightStruct->Range = 5;
}

// Constructor - Set up a point light
PointLight::PointLight(bool castShadows, float radius, XMFLOAT3 color, float intensity) :
	Light::Light(LightType::PointLight, castShadows, color, intensity)
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

// Calculate view for shadow rendering
void PointLight::CalculateViewMatrix()
{
	//TODO: Implement point light shadow maps
	printf("Point light shadow maps are not supported (yet)");
}

// Calculate projection for shadow rendering
void PointLight::CalculateProjMatrix()
{
	//TODO: Implement point light shadow maps
	printf("Point light shadow maps are not supported (yet)");
}

// Get this light's view matrix (for shadows)
DirectX::XMFLOAT4X4 PointLight::GetViewMatrix()
{
	//TODO: Calculate view for point lights
	//TODO: Calculate matrices only when light changes
	printf("Point light shadow maps are not supported (yet)");
	return shadowView;
}

// Get this light's projection matrix (for shadows)
DirectX::XMFLOAT4X4 PointLight::GetProjectionMatrix()
{
	//TODO: Calculate proj for point lights
	//TODO: Calculate matrices only when light changes
	printf("Point light shadow maps are not supported (yet)");
	return shadowProj;
}
#pragma endregion


#pragma region Spot Light
// ----------------------------------------------------------------------------
//									 SPOT LIGHT
// ----------------------------------------------------------------------------

// Constructor - Set up a spot light with default values.
SpotLight::SpotLight(bool castShadows) : Light::Light(LightType::SpotLight, castShadows)
{
	lightStruct->SpotFalloff = 5;
	lightStruct->Range = 5;
}

// Constructor - Set up a spot light
SpotLight::SpotLight(bool castShadows, float range, float spotFalloff, XMFLOAT3 color, float intensity) :
	Light::Light(LightType::SpotLight, castShadows, color, intensity)
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

// Calculate view for shadow rendering
void SpotLight::CalculateViewMatrix()
{
	//TODO: Implement Spot light shadow maps
	printf("Spot light shadow maps are not supported (yet)");
}

// Calculate projection for shadow rendering
void SpotLight::CalculateProjMatrix()
{
	//TODO: Implement Spot light shadow maps
	printf("Spot light shadow maps are not supported (yet)");
}

// Get this light's view matrix (for shadows)
DirectX::XMFLOAT4X4 SpotLight::GetViewMatrix()
{
	//TODO: Calculate view for Spot lights
	//TODO: Calculate matrices only when light changes
	printf("Spot light shadow maps are not supported (yet)");
	return shadowView;
}

// Get this light's projection matrix (for shadows)
DirectX::XMFLOAT4X4 SpotLight::GetProjectionMatrix()
{
	//TODO: Calculate proj for point lights
	//TODO: Calculate matrices only when light changes
	printf("Spot light shadow maps are not supported (yet)");
	return shadowProj;
}
#pragma endregion

//Set the light manager's light list to dirty
// THIS FRIEND FUNCTION CAN ONLY BE ACCESSED BY THE LIGHT
//		IN Lights.cpp
void SetLightListDirty(LightManager* lightManager) {
	lightManager->listDirty = true;
}