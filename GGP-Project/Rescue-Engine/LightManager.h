#pragma once
#include "Lights.h"
#include <vector>

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

	//Light struct array helpers
	bool listDirty;
	LightStruct** lightStructArr;

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
	DirectionalLight* CreateDirectionalLight();

	// --------------------------------------------------------
	// Create a new directional light and add it to the light manager
	//
	// color - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	DirectionalLight* CreateDirectionalLight(DirectX::XMFLOAT3 color, float intensity);

	// --------------------------------------------------------
	// Create a new point light and add it to the light manager
	// --------------------------------------------------------
	PointLight* CreatePointLight();

	// --------------------------------------------------------
	// Create a new point light and add it to the light manager
	//
	// radius - radius of the point light's sphere
	// color - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	PointLight* CreatePointLight(float radius, DirectX::XMFLOAT3 color, float intensity);

	// --------------------------------------------------------
	// Create a new spot light and add it to the light manager
	// --------------------------------------------------------
	SpotLight* CreateSpotLight();

	// --------------------------------------------------------
	// Create a new spot light and add it to the light manager
	//
	// range - the range of this spotlight
	// spotFallOff - the falloff for this spotlight
	// color - Diffuse color of the light
	// intensity - how intense the light is
	// --------------------------------------------------------
	SpotLight* CreateSpotLight(float range, float spotFalloff, DirectX::XMFLOAT3 color, float intensity);

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
	void GetLightStructArray(LightStruct** arr);
};

