#include "MAT_Water.h"

// Constructor - Set up a material
MAT_Water::MAT_Water(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader,
	float shininess, DirectX::XMFLOAT2 uvScale,
	ID3D11ShaderResourceView* albedo, ID3D11ShaderResourceView* normals,
	ID3D11ShaderResourceView* roughness, ID3D11ShaderResourceView* metal,
	ID3D11SamplerState* sampler, ID3D11SamplerState* shadowSampler, float* translate)
	: MAT_PBRTexture(vertexShader, pixelShader, shininess, uvScale, albedo, normals, roughness,  metal, sampler, shadowSampler)
{
	this->translate = translate;
}

MAT_Water::~MAT_Water()
{
}

void MAT_Water::PrepareMaterialObject(GameObject * entityObj)
{
	//Sends the translate value to the pixel shader
	pixelShader->SetFloat("Translate", *translate);
	pixelShader->CopyBufferData("perObject");
	MAT_PBRTexture::PrepareMaterialObject(entityObj);
}
