#include "MAT_PBRTexture.h"
#include "LightManager.h"
#include "MAT_Basic.h"

// Constructor - Set up a material
MAT_Basic::MAT_Basic(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader,
	DirectX::XMFLOAT2 uvScale, ID3D11SamplerState* sampler,
	ID3D11ShaderResourceView* albedo, ID3D11ShaderResourceView* normals,
	float roughness, float shininess, ID3D11SamplerState* shadowSampler)
	: Material(vertexShader, pixelShader)
{
	this->albedoSRV = albedo;
	this->normalSRV = normals;
	this->shininess = shininess;
	this->roughness = roughness;
	this->sampler = sampler;
	this->uvScale = uvScale;
	this->shadowSampler = shadowSampler;
}

// Release all data in the material
MAT_Basic::~MAT_Basic()
{ }

// Prepare this material's shader's per MatMesh combo variables
void MAT_Basic::PrepareMaterialCombo(GameObject* entityObj, Camera* cam)
{
	LightManager* lightManager = LightManager::GetInstance();
	std::vector<Light*> lights = LightManager::GetInstance()->GetShadowCastingLights();

	// Vertex shader data
	vertexShader->SetMatrix4x4("projection", cam->GetProjectionMatrix());
	vertexShader->SetMatrix4x4("view", cam->GetViewMatrix());
	vertexShader->SetFloat2("uvScale", uvScale);
	vertexShader->SetMatrix4x4("shadowView", lights[0]->GetViewMatrix());
	vertexShader->SetMatrix4x4("shadowProj", lights[0]->GetProjectionMatrix());

	//Pixel shader data
	pixelShader->SetFloat3("CameraPosition", cam->GetPosition());
	pixelShader->SetFloat("Shininess", shininess);
	pixelShader->SetFloat("Roughness", roughness);

	//Set lights
	int amnt = lightManager->GetLightAmnt();
	LightStruct* arr = lightManager->GetLightStructArray();
	pixelShader->SetData("Lights", (void*)arr,
		sizeof(LightStruct) * MAX_LIGHTS);
	pixelShader->SetInt("LightCount", amnt);
	pixelShader->SetData("AmbLight", lightManager->GetAmbientLight(), sizeof(AmbientLightStruct));

	//Set PBR vars
	pixelShader->SetShaderResourceView("AlbedoTexture", albedoSRV);
	pixelShader->SetShaderResourceView("NormalTexture", normalSRV);
	pixelShader->SetSamplerState("BasicSampler", sampler);

	//Set shadow vars
	ID3D11ShaderResourceView* shadowSRV = lights[0]->GetShadowSRV();
	pixelShader->SetShaderResourceView("ShadowMap", shadowSRV);
	pixelShader->SetSamplerState("ShadowSampler", shadowSampler);

	vertexShader->CopyBufferData("perCombo");
	pixelShader->CopyBufferData("perCombo");
}

// Prepare this material's shader's per object variables
void MAT_Basic::PrepareMaterialObject(GameObject* entityObj)
{
	vertexShader->SetMatrix4x4("world", entityObj->GetWorldMatrix());
	vertexShader->SetMatrix4x4("worldInvTrans", entityObj->GetWorldInvTransMatrix());
	vertexShader->CopyBufferData("perObject");
}