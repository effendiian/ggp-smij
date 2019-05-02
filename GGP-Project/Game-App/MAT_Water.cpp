#include "MAT_Water.h"

// Constructor - Set up a material
MAT_Water::MAT_Water(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader,
	DirectX::XMFLOAT2 uvScale, ID3D11SamplerState* sampler,
	ID3D11ShaderResourceView* albedo, ID3D11ShaderResourceView* normals,
	float roughness, float shininess, ID3D11SamplerState* shadowSampler, float* translate,
	ID3D11ShaderResourceView* normals2, ID3D11ShaderResourceView* shineSRV)
	: MAT_Basic(vertexShader, pixelShader, uvScale, sampler, albedo, normals, roughness, shininess, shadowSampler)
{
	this->translate = translate;
	this->normal2SRV = normals2;
	this->shineSRV = shineSRV;
}

MAT_Water::~MAT_Water()
{
}

void MAT_Water::PrepareMaterialCombo(GameObject* entityObj, Camera* cam)
{
	pixelShader->SetShaderResourceView("NormalTexture2", normal2SRV);
	pixelShader->SetShaderResourceView("ShineTexture", shineSRV);
	//pixelShader->SetShaderResourceView("ShineTexture2", shineSRV);
	MAT_Basic::PrepareMaterialCombo(entityObj, cam);
}

void MAT_Water::PrepareMaterialObject(GameObject * entityObj)
{
	//Sends the translate value to the pixel shader
	pixelShader->SetFloat("Translate", *translate);
	pixelShader->CopyBufferData("perObject");
	MAT_Basic::PrepareMaterialObject(entityObj);
}
