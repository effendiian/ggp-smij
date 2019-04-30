#pragma once
#include "Lights.h"
#include <vector>

#define SHADOW_MAP_SIZE 2048

class LightManager
{
private:
	// --------------------------------------------------------
	// Singleton Constructor - Set up the singleton instance of the LightManager
	// --------------------------------------------------------
	LightManager() { Init(); }
	~LightManager();

	// --------------------------------------------------------
	// Initialize values in the LightManager
	// --------------------------------------------------------
	void Init();

	//Lighting containers
	AmbientLightStruct* ambientLight;
	std::vector<Light*> lightList;
	std::vector<Light*> shadowLightList;

	//Light struct array helpers
	bool listDirty;
	LightStruct* lightStructArr;

	//Shadow descs
	D3D11_TEXTURE2D_DESC shadowTexDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC shadowDSDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shadowSRVDesc;

	// --------------------------------------------------------
	//Set the light manager's light list to dirty
	// THIS FRIEND FUNCTION CAN ONLY BE ACCESSED BY THE LIGHT
	//		IN Lights.cpp
	// --------------------------------------------------------
	friend void SetLightListDirty(LightManager* lightManager);

	// --------------------------------------------------------
	// Rebuild the light struct array and the shadow light list
	// --------------------------------------------------------
	void RebuildLightLists();

	// --------------------------------------------------------
	// Rebuild the shadow light list
	// --------------------------------------------------------
	void RebuildShadowLightList();

	// --------------------------------------------------------
	// Rebuild the light struct array from all lights in the lightList
	// --------------------------------------------------------
	void RebuildLightStructArray();

public:
	// --------------------------------------------------------
	// Get the singleton instance of the LightManager
	// --------------------------------------------------------
	static LightManager* GetInstance()
	{
		static LightManager instance;

		return &instance;
	}

	//Delete this
	LightManager(LightManager const&) = delete;
	void operator=(LightManager const&) = delete;

	// --------------------------------------------------------
	// Create a new directional light and add it to the light manager
	// --------------------------------------------------------
	DirectionalLight* CreateDirectionalLight(bool castShadows);

	// --------------------------------------------------------
	// Create a new directional light and add it to the light manager
	//
	// color - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	DirectionalLight* CreateDirectionalLight(bool castShadows, DirectX::XMFLOAT3 color, float intensity);

	// --------------------------------------------------------
	// Create a new point light and add it to the light manager
	// --------------------------------------------------------
	PointLight* CreatePointLight(bool castShadows);

	// --------------------------------------------------------
	// Create a new point light and add it to the light manager
	//
	// radius - radius of the point light's sphere
	// color - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	PointLight* CreatePointLight(bool castShadows, float radius, DirectX::XMFLOAT3 color, float intensity);

	// --------------------------------------------------------
	// Create a new spot light and add it to the light manager
	// --------------------------------------------------------
	SpotLight* CreateSpotLight(bool castShadows);

	// --------------------------------------------------------
	// Create a new spot light and add it to the light manager
	//
	// range - the range of this spotlight
	// spotFallOff - the falloff for this spotlight
	// color - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	SpotLight* CreateSpotLight(bool castShadows, float range, float spotFalloff, DirectX::XMFLOAT3 color, float intensity);

	// --------------------------------------------------------
	// Add a light to the light manager
	// --------------------------------------------------------
	void AddLight(Light* light);

	// --------------------------------------------------------
	// Remomve a light from the light manager
	//
	// deleteLight - whether to delete the light reference when removing
	// --------------------------------------------------------
	void RemoveLight(Light* light, bool deleteLight = true);

	// --------------------------------------------------------
	// Check if the light already exists in the light list
	// --------------------------------------------------------
	bool IsInLightList(Light* light);

	// --------------------------------------------------------
	// Set the color of the ambient light
	// --------------------------------------------------------
	void SetAmbientColor(DirectX::XMFLOAT3 color);

	// --------------------------------------------------------
	// Set the color of the ambient light
	//
	// r - red
	// g - green
	// b - blue
	// --------------------------------------------------------
	void SetAmbientColor(float r, float g, float b);

	// --------------------------------------------------------
	// Set the intensity of the ambient light
	// --------------------------------------------------------
	void SetAmbientIntensity(float intensity);

	// --------------------------------------------------------
	// Get the ambient light
	// --------------------------------------------------------
	AmbientLightStruct* GetAmbientLight();

	// --------------------------------------------------------
	// Get the amount of lights in the manager
	// --------------------------------------------------------
	int GetLightAmnt();

	// --------------------------------------------------------
	// Get the array of light structs for sending to a shader
	// --------------------------------------------------------
	LightStruct* GetLightStructArray();

	// --------------------------------------------------------
	// Get all lights that cast shadows
	// --------------------------------------------------------
	std::vector<Light*> GetShadowCastingLights();

	// --------------------------------------------------------
	// Get the shadow texture description for creating shadowTexs
	// --------------------------------------------------------
	D3D11_TEXTURE2D_DESC* GetShadowTexDesc();

	// --------------------------------------------------------
	// Get the shadow depth/stencil description for creating shadowDSs
	// --------------------------------------------------------
	D3D11_DEPTH_STENCIL_VIEW_DESC* GetShadowDSDesc();

	// --------------------------------------------------------
	// Get the shadow resource view description for creating shadowSRVs
	// --------------------------------------------------------
	D3D11_SHADER_RESOURCE_VIEW_DESC* GetShadowSRVDesc();
};

