#include "MAT_Skybox.h"

MAT_Skybox::MAT_Skybox(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader,
	ID3D11ShaderResourceView* skySRV, ID3D11SamplerState* sampler) 
	: Material(vertexShader, pixelShader)
{
	this->skySRV = skySRV;
	this->sampler = sampler;
}

MAT_Skybox::~MAT_Skybox()
{ }

void MAT_Skybox::PrepareMaterialCombo(GameObject* entityObj, Camera* camera)
{
	vertexShader->SetMatrix4x4("view", camera->GetViewMatrix());
	vertexShader->SetMatrix4x4("projection", camera->GetProjectionMatrix());
	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();

	pixelShader->SetShaderResourceView("Sky", skySRV);
	pixelShader->SetSamplerState("BasicSampler", sampler);
	pixelShader->SetShader();
}
