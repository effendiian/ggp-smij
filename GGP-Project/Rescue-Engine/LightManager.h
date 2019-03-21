#pragma once
#include "Lights.h"

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
	DirectX::XMFLOAT4 ambientColor;

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

	void
};

