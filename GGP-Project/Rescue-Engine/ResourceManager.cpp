#include "ResourceManager.h"
#include <sstream>

using namespace DirectX;

ResourceManager::~ResourceManager()
{
	//Delete Texture2Ds
	for (auto const& pair : texture2DMap)
	{
		if (pair.second) { pair.second->Release(); }
	}
	texture2DMap.clear();

	//Delete Cubemaps
	for (auto const& pair : cubemapMap)
	{
		if (pair.second) { pair.second->Release(); }
	}
	cubemapMap.clear();

	//Delete Meshes
	for (auto const& pair : meshMap)
	{
		if (pair.second) { delete pair.second; }
	}
	meshMap.clear();

	//Delete Pixel Shaders
	for (auto const& pair : pixelShaderMap)
	{
		if (pair.second) { delete pair.second; }
	}
	pixelShaderMap.clear();

	//Delete Vertex Shaders
	for (auto const& pair : vertexShaderMap)
	{
		if (pair.second) { delete pair.second; }
	}
	vertexShaderMap.clear();

	//Delete Materials
	for (auto const& pair : materialMap)
	{
		if (pair.second) { delete pair.second; }
	}
	materialMap.clear();
}

// Load a Texture2D from the specified address with MipMaps
bool ResourceManager::LoadTexture2D(const char* address, ID3D11Device* device, ID3D11DeviceContext* context)
{
	//Create a string
	std::stringstream ss;
	ss << address;
	std::string str = ss.str();
	std::wstring lStr = std::wstring(str.begin(), str.end());
	const wchar_t* lAddress = lStr.c_str();

	//Check if the Texture2D is already in the map
	if (texture2DMap.find(str) != texture2DMap.end())
	{
		printf("Texture2D at address \"%s\" already exists in the resource manager\n", address);
		return false;
	}

	//Load the Texture2D
	ID3D11ShaderResourceView* tex;
	if (CreateWICTextureFromFile(device, context, lAddress, 0, &tex) != S_OK)
	{
		printf("Could not load texture2D %s\n", address);
		return false;
	}

	//Add to map
	texture2DMap.emplace(str, tex);
	return true;
}

// Load a Texture2D from the specified address with NO MipMaps
bool ResourceManager::LoadTexture2D(const char* address, ID3D11Device * device)
{
	//Create a string
	std::stringstream ss;
	ss << address;
	std::string str = ss.str();
	std::wstring lStr = std::wstring(str.begin(), str.end());
	const wchar_t* lAddress = lStr.c_str();

	//Check if the Texture2D is already in the map
	if (texture2DMap.find(str) != texture2DMap.end())
	{
		printf("Texture2D at address \"%s\" already exists in the resource manager\n", address);
		return false;
	}

	//Load the Texture2D
	ID3D11ShaderResourceView* tex;
	if (CreateWICTextureFromFile(device, lAddress, 0, &tex) != S_OK)
	{
		printf("Could not load Texture2D \"%s\"\n", address);
		return false;
	}

	//Add to map
	texture2DMap.emplace(str, tex);
	return true;
}

// Load a CubeMap from the specified address with MipMaps
bool ResourceManager::LoadCubeMap(const char* address, ID3D11Device* device, ID3D11DeviceContext* context)
{
	//Create a string
	std::stringstream ss;
	ss << address;
	std::string str = ss.str();
	std::wstring lStr = std::wstring(str.begin(), str.end());
	const wchar_t* lAddress = lStr.c_str();

	//Check if the CubeMap is already in the map
	if (cubemapMap.find(str) != cubemapMap.end())
	{
		printf("CubeMap at address \"%s\" already exists in the resource manager\n", address);
		return false;
	}

	//Load the Texture2D
	ID3D11ShaderResourceView* tex;
	if(CreateDDSTextureFromFile(device, context, lAddress, 0, &tex) != S_OK)
	{
		printf("Could not load CubeMap \"%s\"\n", address);
		return false;
	}

	//Add to map
	cubemapMap.emplace(str, tex);
	return true;
}

// Load a CubeMap from the specified address with NO MipMaps
bool ResourceManager::LoadCubeMap(const char* address, ID3D11Device* device)
{
	//Create a string
	std::stringstream ss;
	ss << address;
	std::string str = ss.str();
	std::wstring lStr = std::wstring(str.begin(), str.end());
	const wchar_t* lAddress = lStr.c_str();

	//Check if the CubeMap is already in the map
	if (cubemapMap.find(str) != cubemapMap.end())
	{
		printf("CubeMap at address \"%s\" already exists in the resource manager\n", address);
		return false;
	}

	//Load the Texture2D
	ID3D11ShaderResourceView* tex;
	if (CreateDDSTextureFromFile(device, lAddress, 0, &tex) != S_OK)
	{
		printf("Could not load CubeMap \"%s\"\n", address);
		return false;
	}

	//Add to map
	cubemapMap.emplace(str, tex);
	return true;
}

// Load a Mesh from the specified address
bool ResourceManager::LoadMesh(const char* address, ID3D11Device* device)
{
	//Create a string
	std::stringstream ss;
	ss << address;
	std::string str = ss.str();

	//Check if the Mesh is already in the map
	if (meshMap.find(str) != meshMap.end())
	{
		printf("Mesh at address \"%s\" already exists in the resource manager\n", address);
		return false;
	}

	//Load the Mesh
	Mesh* mesh = new Mesh(address, device);
	if (!mesh->IsMeshLoaded()) 
	{
		printf("Could not load Mesh \"%s\"\n", address);
		if (mesh) { delete mesh; }
		return false;
	}

	//Add to map
	meshMap.emplace(str, mesh);
	return true;
}

// Load a Material from the specified address
bool ResourceManager::AddMaterial(const char* name, Material* material)
{
	//Create a string
	std::stringstream ss;
	ss << name;
	std::string str = ss.str();

	//Check if the Material is already in the map
	if (materialMap.find(str) != materialMap.end())
	{
		printf("Material of name \"%s\" already exists in the resource manager\n", name);
		return false;
	}
	
	//Add to map
	materialMap.emplace(str, material);
	return true;
}

// Load a Pixel Shader from the specified address
bool ResourceManager::LoadPixelShader(const char* name, ID3D11Device* device, ID3D11DeviceContext* context)
{
	//Create a string
	std::stringstream ss;
	ss << name;
	std::string str = ss.str();
	std::wstring lStr = std::wstring(str.begin(), str.end());
	const wchar_t* lName = lStr.c_str();

	//Check if the Pixel Shader is already in the map
	if (pixelShaderMap.find(str) != pixelShaderMap.end())
	{
		printf("Pixel Shader of name \"%s\" already exists in the resource manager\n", name);
		return false;
	}

	//Load shader
	SimplePixelShader* ps = new SimplePixelShader(device, context);
	if (!ps->LoadShaderFile(lName))
	{
		printf("Could not load Pixel Shader \"%s\"\n", name);
		return false;
	}

	//Add to map
	pixelShaderMap.emplace(str, ps);
	return false;
}

// Load a Vertex Shader from the specified address
bool ResourceManager::LoadVertexShader(const char* name, ID3D11Device* device, ID3D11DeviceContext* context)
{
	//Create a string
	std::stringstream ss;
	ss << name;
	std::string str = ss.str();
	std::wstring lStr = std::wstring(str.begin(), str.end());
	const wchar_t* lName = lStr.c_str();

	//Check if the Vertex Shader is already in the map
	if (vertexShaderMap.find(str) != vertexShaderMap.end())
	{
		printf("Vertex Shader of name \"%s\" already exists in the resource manager\n", name);
		return false;
	}

	//Load shader
	SimpleVertexShader* vs = new SimpleVertexShader(device, context);
	if (!vs->LoadShaderFile(lName))
	{
		printf("Could not load Vertex Shader \"%s\"\n", name);
		return false;
	}

	//Add to map
	vertexShaderMap.emplace(str, vs);
	return false;
}

// Get a loaded Texture2D
ID3D11ShaderResourceView* ResourceManager::GetTexture2D(std::string address)
{
	//Check if the Texture2D is in the map
	if (texture2DMap.find(address) == texture2DMap.end())
	{
		printf("Texture2D at address \"%s\" does not exist in the resource manager\n", address.c_str());
		return nullptr;
	}

	return texture2DMap[address];
}

// Get a loaded CubeMap
ID3D11ShaderResourceView* ResourceManager::GetCubeMap(std::string address)
{
	//Check if the CubeMap is in the map
	if (cubemapMap.find(address) == cubemapMap.end())
	{
		printf("CubeMap at address \"%s\" does not exist in the resource manager\n", address.c_str());
		return nullptr;
	}

	return cubemapMap[address];
}

// Get a loaded Mesh
Mesh* ResourceManager::GetMesh(std::string address)
{
	//Check if the Mesh is in the map
	if (meshMap.find(address) == meshMap.end())
	{
		printf("Mesh at address \"%s\" does not exist in the resource manager\n", address.c_str());
		return nullptr;
	}

	return meshMap[address];
}

// Get a added Material
Material* ResourceManager::GetMaterial(std::string name)
{
	//Check if the Material is in the map
	if (materialMap.find(name) == materialMap.end())
	{
		printf("Material of name \"%s\" does not exist in the resource manager\n", name.c_str());
		return nullptr;
	}

	return materialMap[name];
}

// Get a loaded Pixel Shader
SimplePixelShader* ResourceManager::GetPixelShader(std::string name)
{
	//Check if the Pixel Shader is in the map
	if (pixelShaderMap.find(name) == pixelShaderMap.end())
	{
		printf("Pixel Shader of name \"%s\" does not exist in the resource manager\n", name.c_str());
		return nullptr;
	}

	return pixelShaderMap[name];
}

// Get a loaded Vertex Shader
SimpleVertexShader* ResourceManager::GetVertexShader(std::string name)
{
	//Check if the Vertex Shader is in the map
	if (vertexShaderMap.find(name) == vertexShaderMap.end())
	{
		printf("Vertex Shader of name \"%s\" does not exist in the resource manager\n", name.c_str());
		return nullptr;
	}

	return vertexShaderMap[name];
}
