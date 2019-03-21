#include "MAT_PBRTexture.h"

// Constructor - Set up a material
MAT_PBRTexture::MAT_PBRTexture(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader,
	float shininess, DirectX::XMFLOAT2 uvScale, 
	ID3D11ShaderResourceView* albedo, ID3D11ShaderResourceView* normals, 
	ID3D11ShaderResourceView* roughness, ID3D11ShaderResourceView* metal, 
	ID3D11SamplerState* sampler)
	: Material(vertexShader, pixelShader)
{
	this->shininess = shininess;
	this->albedoSRV = albedo;
	this->normalSRV = normals;
	this->roughnessSRV = roughness;
	this->metalSRV = metal;
	this->sampler = sampler;
	this->uvScale = uvScale;
}

// Release all data in the material
MAT_PBRTexture::~MAT_PBRTexture()
{
}

// Prepare this material's shader's per MatMesh combo variables
void MAT_PBRTexture::PrepareMaterialCombo(GameObject* entityObj, Camera* cam)
{
	// Turn shaders on
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Vertex shader data
	vertexShader->SetMatrix4x4("projection", cam->GetProjectionMatrix());
	vertexShader->SetMatrix4x4("view", cam->GetViewMatrix());
	vertexShader->CopyBufferData("perCombo");

	//Pixel shader data
	pixelShader->SetFloat3("CameraPosition", cam->GetPosition());
	pixelShader->SetFloat("LightCount", 0);
	pixelShader->CopyBufferData("perCombo");

	pixelShader->SetShaderResourceView("AlbedoTexture", albedoSRV);
	pixelShader->SetShaderResourceView("NormalTexture", normalSRV);
	pixelShader->SetShaderResourceView("RoughnessTexture", roughnessSRV);
	pixelShader->SetShaderResourceView("MetalTexture", metalSRV);
	pixelShader->SetSamplerState("BasicSampler", sampler);
}

// Prepare this material's shader's per object variables
void MAT_PBRTexture::PrepareMaterialObject(GameObject* entityObj)
{
	vertexShader->SetMatrix4x4("world", entityObj->GetWorldMatrix());
	vertexShader->SetMatrix4x4("worldInvTrans", entityObj->GetWorldInvTransMatrix());
	vertexShader->CopyBufferData("perObject");
}