#include "LightManager.h"
#include <algorithm>

using namespace DirectX;

LightManager::~LightManager()
{
	if (ambientLight) { delete ambientLight; }
	for (int i = 0; i < lightList.size(); i++)
	{
		if (lightList[i]) { delete lightList[i]; }
	}
	if (lightStructArr) { delete[] lightStructArr; }
}

// Initialize values in the LightManager
void LightManager::Init()
{
	listDirty = true;
	ambientLight = new AmbientLightStruct();
	ambientLight->Color = XMFLOAT3(0, 0, 0);
	ambientLight->Intensity = 1;
}

// Create a new directional light and add it to the light manager
DirectionalLight* LightManager::CreateDirectionalLight()
{
	if (lightList.size() >= MAX_LIGHTS)
	{
		printf("Can't create more lights. MAX_LIGHTS (%d) reached", MAX_LIGHTS);
		return nullptr;
	}

	listDirty = true;
	DirectionalLight* light = new DirectionalLight();
	SetInLightManager(light, true);
	lightList.push_back(light);
	return light;
}

// Create a new directional light and add it to the light manager
DirectionalLight* LightManager::CreateDirectionalLight(XMFLOAT3 color, float intensity)
{
	if (lightList.size() >= MAX_LIGHTS)
	{
		printf("Can't create more lights. MAX_LIGHTS (%d) reached", MAX_LIGHTS);
		return nullptr;
	}

	listDirty = true;
	DirectionalLight* light = new DirectionalLight(color, intensity);
	SetInLightManager(light, true);
	lightList.push_back(light);
	return light;
}

// Create a new point light and add it to the light manager
PointLight* LightManager::CreatePointLight()
{
	if (lightList.size() >= MAX_LIGHTS)
	{
		printf("Can't create more lights. MAX_LIGHTS (%d) reached", MAX_LIGHTS);
		return nullptr;
	}

	listDirty = true;
	PointLight* light = new PointLight();
	SetInLightManager(light, true);
	lightList.push_back(light);
	return light;
}

// Create a new point light and add it to the light manager
PointLight* LightManager::CreatePointLight(float radius, XMFLOAT3 color, float intensity)
{
	if (lightList.size() >= MAX_LIGHTS)
	{
		printf("Can't create more lights. MAX_LIGHTS (%d) reached", MAX_LIGHTS);
		return nullptr;
	}

	listDirty = true;
	PointLight* light = new PointLight(radius, color, intensity);
	SetInLightManager(light, true);
	lightList.push_back(light);
	return light;
}

// Create a new spot light and add it to the light manager
SpotLight* LightManager::CreateSpotLight()
{
	if (lightList.size() >= MAX_LIGHTS)
	{
		printf("Can't create more lights. MAX_LIGHTS (%d) reached", MAX_LIGHTS);
		return nullptr;
	}

	listDirty = true;
	SpotLight* light = new SpotLight();
	SetInLightManager(light, true);
	lightList.push_back(light);
	return light;
}

// Create a new spot light and add it to the light manager
SpotLight* LightManager::CreateSpotLight(float range, float spotFalloff, XMFLOAT3 color, float intensity)
{
	if (lightList.size() >= MAX_LIGHTS)
	{
		printf("Can't create more lights. MAX_LIGHTS (%d) reached", MAX_LIGHTS);
		return nullptr;
	}

	listDirty = true;
	SpotLight* light = new SpotLight(range, spotFalloff, color, intensity);
	SetInLightManager(light, true);
	lightList.push_back(light);
	return light;
}

// Add a light to the light manager
void LightManager::AddLight(Light* light)
{
	if (lightList.size() >= MAX_LIGHTS)
	{
		printf("Can't create more lights. MAX_LIGHTS (%d) reached", MAX_LIGHTS);
		return;
	}

	if (IsInLightList(light))
	{
		printf("Light of type %d already exists in light list. Cannot add", light->GetType());
		return;
	}

	listDirty = true;
	SetInLightManager(light, true);
	lightList.push_back(light);
}

// Remomve a light from the light manager
void LightManager::RemoveLight(Light* light, bool deleteLight)
{
	//Get iterator
	std::vector<Light*>::iterator it = std::find(lightList.begin(), lightList.end(), light);
	
	if (it != lightList.end())
	{
		listDirty = true;
		Light* light = *it;

		//Swap it for the last one
		std::swap(*it, lightList[lightList.size() - 1]);

		//Pop the last one
		lightList.pop_back();

		//Delete instance if user wants to
		if (deleteLight)
		{
			delete light;
		}
		else SetInLightManager(light, false);
	}
	else
	{
		printf("Light of type %d does not exist in light list. Cannot remove", light->GetType());
		return;
	}
}

// Check if the light already exists in the light list
bool LightManager::IsInLightList(Light* light)
{
	return std::find(lightList.begin(), lightList.end(), light) != lightList.end();
}

// Set the color of the ambient light
void LightManager::SetAmbientColor(XMFLOAT3 color)
{
	ambientLight->Color = color;
}

// Set the color of the ambient light
void LightManager::SetAmbientColor(float r, float g, float b)
{
	ambientLight->Color = XMFLOAT3(r, g, b);
}

// Set the intensity of the ambient light
void LightManager::SetAmbientIntensity(float intensity)
{
	ambientLight->Intensity = intensity;
}

AmbientLightStruct* LightManager::GetAmbientLight()
{
	return ambientLight;
}

// Get the amount of lights in the manager
int LightManager::GetLightAmnt()
{
	return (int)lightList.size();
}

// Get the array of light structs for sending to a shader
LightStruct* LightManager::GetLightStructArray()
{
	if (listDirty)
		RebuildLightStructArray();

	//return lightStructArray[0];
	return lightStructArr;
}

// Rebuild the light struct array from all lights in the lightList
void LightManager::RebuildLightStructArray()
{
	listDirty = false;

	//Reset array
	if (lightStructArr) { delete lightStructArr; }
	lightStructArr = new LightStruct[MAX_LIGHTS];

	//Rebuild
	for (size_t i = 0; i < lightList.size(); i++)
	{
		lightStructArr[i] = *(lightList[i]->GetLightStruct());
	}
}

// Set whether this light is in the light manager.
// THIS FUNCTION CAN ONLY BE ACCESSED BY THE LIGHT MANAGER
//		IN LightManager.cpp
void SetInLightManager(Light* light, bool val)
{
	light->inLightManager = val;
}