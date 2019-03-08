#include "Material.h"

// Constructor - Set up a material
Material::Material(SimpleVertexShader * vertexShader, SimplePixelShader * pixelShader, 
	ID3D11ShaderResourceView* resourceView, ID3D11SamplerState* samplerState)
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
	specularity = 64;
	this->resourceView = resourceView;
	this->samplerState = samplerState;
	surfaceColor = DirectX::XMFLOAT4(1, 1, 1, 1);
}

// Release all data in the material
Material::~Material()
{ }

// Get this materials vertex shas=der
SimpleVertexShader* Material::GetVertexShader()
{
	return vertexShader;
}

// Get this materials pixel shader
SimplePixelShader* Material::GetPixelShader()
{
	return pixelShader;
}

// Get this material's resource view
ID3D11ShaderResourceView* Material::GetResourceView()
{
	return resourceView;
}

// Get this material's sampler state
ID3D11SamplerState* Material::GetSamplerState()
{
	return samplerState;
}

// Set this materials surface color
void Material::SetSurfaceColor(DirectX::XMFLOAT4 color)
{
	surfaceColor = color;
}

// Get this materials surface color
DirectX::XMFLOAT4 Material::GetSurfaceColor()
{
	return surfaceColor;
}

void Material::SetSpecularity(float spec)
{
	specularity = spec;
}

float Material::GetSpecularity()
{
	return specularity;
}
