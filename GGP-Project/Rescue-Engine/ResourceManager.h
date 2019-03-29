#pragma once
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include <map>
#include "Mesh.h"
#include "Material.h"

class ResourceManager
{
private:
	// --------------------------------------------------------
	// Singleton Constructor - Set up the singleton instance of the ResourceManager
	// --------------------------------------------------------
	ResourceManager() {}
	~ResourceManager();

	//Resource maps
	std::map<std::wstring, ID3D11ShaderResourceView*> texture2DMap;
	std::map<std::wstring, ID3D11ShaderResourceView*> cubemapMap;
	std::map<std::string, Mesh*> meshMap;
	std::map<std::string, Material*> materialMap;
	std::map<std::wstring, SimplePixelShader*> pixelShaderMap;
	std::map<std::wstring, SimpleVertexShader*> vertexShaderMap;

public:
	// --------------------------------------------------------
	// Get the singleton instance of the ResourceManager
	// --------------------------------------------------------
	static ResourceManager* GetInstance()
	{
		static ResourceManager instance;

		return &instance;
	}

	//Delete this
	ResourceManager(ResourceManager const&) = delete;
	void operator=(ResourceManager const&) = delete;

	// --------------------------------------------------------
	// Load a Texture2D from the specified address with MipMaps
	// --------------------------------------------------------
	bool LoadTexture2D(const char* address, ID3D11Device* device, ID3D11DeviceContext* context);

	// --------------------------------------------------------
	// Load a Texture2D from the specified address with NO MipMaps
	// --------------------------------------------------------
	bool LoadTexture2D(const char* address, ID3D11Device* device);

	// --------------------------------------------------------
	// Load a CubeMap from the specified address with MipMaps
	// --------------------------------------------------------
	bool LoadCubeMap(const char* address, ID3D11Device* device, ID3D11DeviceContext* context);

	// --------------------------------------------------------
	// Load a CubeMap from the specified address with NO MipMaps
	// --------------------------------------------------------
	bool LoadCubeMap(const char* address, ID3D11Device* device);

	// --------------------------------------------------------
	// Load a Mesh from the specified address
	// --------------------------------------------------------
	bool LoadMesh(const char* address, ID3D11Device* device);

	// --------------------------------------------------------
	// Add an existing Material to the manager
	// --------------------------------------------------------
	bool AddMaterial(const char* name, Material* material);

	// --------------------------------------------------------
	// Load a Pixel Shader from the specified address
	// --------------------------------------------------------
	bool LoadPixelShader(const char* name, ID3D11Device* device, ID3D11DeviceContext* context);

	// --------------------------------------------------------
	// Load a Vertex Shader from the specified address
	// --------------------------------------------------------
	bool LoadVertexShader(const char* name, ID3D11Device* device, ID3D11DeviceContext* context);

	// --------------------------------------------------------
	// Get a loaded Texture2D
	//
	// address - The file address of the Texture2D
	// --------------------------------------------------------
	ID3D11ShaderResourceView* GetTexture2D(std::wstring address);

	// --------------------------------------------------------
	// Get a loaded CubeMap
	//
	// address - The file address of the CubeMap
	// --------------------------------------------------------
	ID3D11ShaderResourceView* GetCubeMap(std::wstring address);

	// --------------------------------------------------------
	// Get a loaded Mesh
	//
	// address - The file address of the Mesh
	// --------------------------------------------------------
	Mesh* GetMesh(std::string address);

	// --------------------------------------------------------
	// Get a added Material
	//
	// name - The name of the Material
	// --------------------------------------------------------
	Material* GetMaterial(std::string name);

	// --------------------------------------------------------
	// Get a loaded Pixel Shader
	//
	// name - The name of the Pixel Shader file
	// --------------------------------------------------------
	SimplePixelShader* GetPixelShader(std::wstring name);

	// --------------------------------------------------------
	// Get a loaded Vertex Shader
	//
	// name - The name of the Vertex Shader file
	// --------------------------------------------------------
	SimpleVertexShader* GetVertexShader(std::wstring name);
};

