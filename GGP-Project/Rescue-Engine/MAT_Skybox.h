#pragma once
#include "Material.h"
class MAT_Skybox :
	public Material
{
public:
	MAT_Skybox(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader,
		ID3D11ShaderResourceView* skySRV, ID3D11SamplerState* sampler);
	~MAT_Skybox();

	ID3D11ShaderResourceView* skySRV;
	ID3D11SamplerState* sampler;

	// --------------------------------------------------------
	// Prepare the sky's shader's shader variables
	// --------------------------------------------------------
	void PrepareMaterialCombo(GameObject* entityObj, Camera* camera) override;

	void PrepareMaterialObject(GameObject* entityObj) override {}
};

