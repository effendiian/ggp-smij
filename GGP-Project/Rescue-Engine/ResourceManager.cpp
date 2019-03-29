#include "ResourceManager.h"

using namespace DirectX;

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Init()
{
}

bool ResourceManager::LoadTexture2D(const wchar_t* address, ID3D11Device* device, ID3D11DeviceContext* context)
{
	ID3D11ShaderResourceView* tex;
	CreateWICTextureFromFile(device, context, address, 0, &tex);

	return true;
}

bool ResourceManager::LoadTexture2D(const wchar_t* address, ID3D11Device * device)
{
	return false;
}

bool ResourceManager::LoadCubeMap(const wchar_t* address)
{
	return false;
}

bool ResourceManager::LoadMesh(const wchar_t* address)
{
	return false;
}

bool ResourceManager::LoadMaterial(const wchar_t* address)
{
	return false;
}

bool ResourceManager::LoadPixelShader(const wchar_t* address)
{
	return false;
}

bool ResourceManager::LoadVertexShader(const wchar_t* address)
{
	return false;
}
