#pragma once
#include "Material.h"
class MAT_Basic :
	public Material
{
private:
	DirectX::XMFLOAT2 uvScale;

	ID3D11ShaderResourceView* albedoSRV;
	ID3D11ShaderResourceView* normalSRV;
	ID3D11SamplerState* sampler;

	float shininess;
	float roughness;

	ID3D11SamplerState* shadowSampler;

public:
	// --------------------------------------------------------
	// Constructor - Set up a material
	//
	// vertexShader - The vertex shader this material uses
	// pixelShader - The pixel shader this material uses
	// --------------------------------------------------------
	MAT_Basic(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader,
		DirectX::XMFLOAT2 uvScale, ID3D11SamplerState* sampler,
		ID3D11ShaderResourceView* albedo, ID3D11ShaderResourceView* normals,
		float roughness, float shininess, ID3D11SamplerState* shadowSampler);

	// --------------------------------------------------------
	// Release all data in the material
	// --------------------------------------------------------
	~MAT_Basic();

	// --------------------------------------------------------
	// Prepare this material's shader's per MatMesh combo variables
	// --------------------------------------------------------
	void PrepareMaterialCombo(GameObject* entityObj, Camera* cam) override;

	// --------------------------------------------------------
	// Prepare this material's shader's per object variables
	// --------------------------------------------------------
	void PrepareMaterialObject(GameObject* entityObj) override;
};

