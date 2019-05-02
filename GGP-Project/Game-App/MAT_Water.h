#pragma once
#include "MAT_Basic.h"
class MAT_Water :
	public MAT_Basic
{
private:
	float* translate; //value that counts from 0.0 to 1.0, keeps track of the scrolling of the normal map
	ID3D11ShaderResourceView* normal2SRV;
	ID3D11ShaderResourceView* shineSRV;

public:
	MAT_Water::MAT_Water(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader,
		DirectX::XMFLOAT2 uvScale, ID3D11SamplerState* sampler,
		ID3D11ShaderResourceView* albedo, ID3D11ShaderResourceView* normals,
		float roughness, float shininess, ID3D11SamplerState* shadowSampler, float* translate, 
		ID3D11ShaderResourceView* normals2, ID3D11ShaderResourceView* shineSRV);
	~MAT_Water();

	void PrepareMaterialCombo(GameObject* entityObj, Camera* cam);

	void PrepareMaterialObject(GameObject* entityObj);
};

