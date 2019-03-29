#pragma once
#include "WICTextureLoader.h"
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
	std::map<std::string, ID3D11ShaderResourceView*> texture2DMap;
	std::map<std::string, ID3D11ShaderResourceView*> cubemapMap;
	std::map<std::string, Mesh*> meshMap;
	std::map<std::string, Material*> materialMap;
	std::map<std::string, SimplePixelShader> pixelShaderMap;
	std::map<std::string, SimpleVertexShader> vertexShaderMap;

public:
	// --------------------------------------------------------
	// Initialize values in the ResourceManager
	// --------------------------------------------------------
	void Init();

	static ResourceManager* GetInstance()
	{
		static ResourceManager instance;

		return &instance;
	}

	//Delete this
	ResourceManager(ResourceManager const&) = delete;
	void operator=(ResourceManager const&) = delete;

	// --------------------------------------------------------
	// Load a texture 2D from the specified address with MipMaps
	// --------------------------------------------------------
	bool LoadTexture2D(const wchar_t* address, ID3D11Device* device, ID3D11DeviceContext* context);

	// --------------------------------------------------------
	// Load a texture 2D from the specified address with NO MipMaps
	// --------------------------------------------------------
	bool LoadTexture2D(const wchar_t* address, ID3D11Device* device);

	// --------------------------------------------------------
	// Load a cubemap from the specified address
	// --------------------------------------------------------
	bool LoadCubeMap(const wchar_t* address);

	// --------------------------------------------------------
	// Load a mesh from the specified address
	// --------------------------------------------------------
	bool LoadMesh(const wchar_t* address);

	// --------------------------------------------------------
	// Load a material from the specified address
	// --------------------------------------------------------
	bool LoadMaterial(const wchar_t* address);

	// --------------------------------------------------------
	// Load a pixel shader from the specified address
	// --------------------------------------------------------
	bool LoadPixelShader(const wchar_t* address);

	// --------------------------------------------------------
	// Load a vertex shader from the specified address
	// --------------------------------------------------------
	bool LoadVertexShader(const wchar_t* address);

};

