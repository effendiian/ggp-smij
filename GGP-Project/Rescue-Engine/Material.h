#pragma once
#include "SimpleShader.h"

// --------------------------------------------------------
// A material definition.
//
// Holds data (shaders) for a material
// --------------------------------------------------------
class Material
{
private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	ID3D11ShaderResourceView* resourceView;
	ID3D11SamplerState* samplerState;
	DirectX::XMFLOAT4 surfaceColor;
	float specularity;

public:
	// --------------------------------------------------------
	// Constructor - Set up a material
	//
	// vertexShader - The vertex shader this material uses
	// pixelShader - The pixel shader this material uses
	// --------------------------------------------------------
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, 
		ID3D11ShaderResourceView* resourceView, ID3D11SamplerState* samplerState);
	
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
	// Get this material's resource view
	// --------------------------------------------------------
	ID3D11ShaderResourceView* GetResourceView();

	// --------------------------------------------------------
	// Get this material's sampler state
	// --------------------------------------------------------
	ID3D11SamplerState* GetSamplerState();

	// --------------------------------------------------------
	// Set this material's surface color
	// --------------------------------------------------------
	void SetSurfaceColor(DirectX::XMFLOAT4 color);

	// --------------------------------------------------------
	// Get this material's surface color
	// --------------------------------------------------------
	DirectX::XMFLOAT4 GetSurfaceColor();

	// --------------------------------------------------------
	// Set this material's specularity
	// --------------------------------------------------------
	void SetSpecularity(float spec);

	// --------------------------------------------------------
	// Get this material's specularity
	// --------------------------------------------------------
	float GetSpecularity();
};

