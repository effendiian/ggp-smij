#pragma once
#include "SimpleShader.h"
#include "GameObject.h"
#include "Camera.h"

// --------------------------------------------------------
// A material definition.
//
// Holds data (shaders) for a material
// --------------------------------------------------------
class Material
{

protected:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// --------------------------------------------------------
	// Constructor - Set up a material
	//
	// vertexShader - The vertex shader this material uses
	// pixelShader - The pixel shader this material uses
	// --------------------------------------------------------
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader);

public:
	// --------------------------------------------------------
	// Release all data in the material
	// --------------------------------------------------------
	~Material();

	// --------------------------------------------------------
	// Get this material's vertex shas=der
	// --------------------------------------------------------
	SimpleVertexShader* GetVertexShader();

	// --------------------------------------------------------
	// Get this material's pixel shader
	// --------------------------------------------------------
	SimplePixelShader* GetPixelShader();

	// --------------------------------------------------------
	// Prepare this material's shader's per MatMesh combo variables
	// --------------------------------------------------------
	virtual void PrepareMaterialCombo(GameObject* entityObj, Camera* cam) = 0;

	// --------------------------------------------------------
	// Prepare this material's shader's per object variables
	// --------------------------------------------------------
	virtual void PrepareMaterialObject(GameObject* entityObj) = 0;
};

