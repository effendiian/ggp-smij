#include "Renderer.h"
#include "LightManager.h"
#include "ResourceManager.h"
#include <algorithm>

#define FXAA_ENABLED 1
#define FXAA_PRESET 5
#define FXAA_DEBUG 0

using namespace DirectX;

// Initialize values in the renderer
void Renderer::Init(ID3D11Device* device, UINT width, UINT height)
{
	// Assign default clear color. We should pull this in from Game at some point.
	this->SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Get collider shader information.
	cubeMesh = ResourceManager::GetInstance()->GetMesh("Assets\\Models\\cube.obj");
	colDebugVS = ResourceManager::GetInstance()->GetVertexShader("VS_ColDebug.cso");
	colDebugPS = ResourceManager::GetInstance()->GetPixelShader("PS_ColDebug.cso");

	//Get skybox information
	skyboxMat = ResourceManager::GetInstance()->GetMaterial("skybox");

	D3D11_RASTERIZER_DESC skyRD = {};
	skyRD.CullMode = D3D11_CULL_FRONT;
	skyRD.FillMode = D3D11_FILL_SOLID;
	skyRD.DepthClipEnable = true;
	device->CreateRasterizerState(&skyRD, &skyRasterState);

	D3D11_DEPTH_STENCIL_DESC skyDS = {};
	skyDS.DepthEnable = true;
	skyDS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	skyDS.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&skyDS, &skyDepthState);

	// Set up the FXAA settings.
	fxaaSettings = new FXAA_DESC();
	fxaaSettings->Init();
	fxaaSettings->LoadPreset(FXAA_PRESET);

#if (FXAA_ENABLED == 0)
	fxaaSettings->FXAA = OFF;
#else
	fxaaSettings->FXAA = ON;
#endif

#if (FXAA_DEBUG == 1)
	// Toggle these on and off as necessary.
	fxaaSettings->DEBUG_DISCARD = ON;
	fxaaSettings->DEBUG_PASSTHROUGH = ON;
	fxaaSettings->DEBUG_HORZVERT = ON;
	fxaaSettings->DEBUG_PAIR = ON;
	fxaaSettings->DEBUG_NEGPOS = ON;
	fxaaSettings->DEBUG_OFFSET = ON;
	fxaaSettings->DEBUG_HIGHLIGHT = ON;
	fxaaSettings->DEBUG_GRAYSCALE = 1.0;
	fxaaSettings->DEBUG_GRAYSCALE_CHANNEL = 1;
#endif

	// Create post-process resources.
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D* ppTexture;
	device->CreateTexture2D(&textureDesc, 0, &ppTexture);

	// Create the Render Target View
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	device->CreateRenderTargetView(ppTexture, &rtvDesc, &fxaaRTV);

	// Create the Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	device->CreateShaderResourceView(ppTexture, &srvDesc, &fxaaSRV);

	// We don't need the texture reference itself no mo'
	ppTexture->Release();

	// Get fxaa shader information.
	fxaaVS = ResourceManager::GetInstance()->GetVertexShader("FXAAShaderVS.cso");
	fxaaPS = ResourceManager::GetInstance()->GetPixelShader("FXAAShaderPS.cso");

	//Wireframe rasterizer state
	D3D11_RASTERIZER_DESC RD_wireframe = {};
	RD_wireframe.FillMode = D3D11_FILL_WIREFRAME;
	RD_wireframe.CullMode = D3D11_CULL_NONE;
	device->CreateRasterizerState(&RD_wireframe, &RS_wireframe);
}

// Destructor for when the singleton instance is deleted
Renderer::~Renderer()
{
	// Clean up rasterizer state.
	RS_wireframe->Release();

	//Clean up skybox
	skyDepthState->Release();
	skyRasterState->Release();

	// Clean up post process.
	fxaaRTV->Release();
	fxaaSRV->Release();
	delete fxaaSettings;
}

// Draw all entities in the render list
void Renderer::Draw(ID3D11DeviceContext* context,
					Camera* camera,
					ID3D11RenderTargetView* backBufferRTV,
					ID3D11DepthStencilView* depthStencilView,
					ID3D11SamplerState* sampler,
					UINT width, UINT height)
{

#pragma region Clear Back Buffer Texture

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, this->clearColor);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

#pragma endregion

	PreparePostProcess(context, fxaaRTV, depthStencilView);

	DrawOpaqueObjects(context, camera);

	DrawSky(context, camera);

	ApplyPostProcess(context, backBufferRTV, depthStencilView, fxaaRTV, fxaaSRV, sampler, width, height);

	DrawDebugColliders(context, camera);

}

// Prepare for post processsing.
void Renderer::PreparePostProcess(ID3D11DeviceContext* context, ID3D11RenderTargetView* ppRTV, ID3D11DepthStencilView* ppDSV)
{
	// POST PROCESS PRE-RENDER ///////////////

		// Clear post process texture.
	context->ClearRenderTargetView(ppRTV, this->clearColor);

	// Set the post process RTV as the current render target.
	context->OMSetRenderTargets(1, &ppRTV, ppDSV);
}

// Draw opaque objects
void Renderer::DrawOpaqueObjects(ID3D11DeviceContext* context, Camera* camera)
{
	//TODO: Apply attenuation
	for (auto const& mapPair : renderMap)
	{
		if (mapPair.second.size() < 1)
			return;

		//Get list, material, and mesh
		std::vector<Entity*> list = mapPair.second;

		//Get the first valid entity
		Entity* firstValid = list[0];

		Material* mat = firstValid->GetMaterial();
		Mesh* mesh = firstValid->GetMesh();

		// Turn shaders on
		mat->GetVertexShader()->SetShader();
		mat->GetPixelShader()->SetShader();

		//Prepare the material's combo specific variables
		mat->PrepareMaterialCombo(firstValid, camera);

		// Set buffers in the input assembler
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		ID3D11Buffer* vertexBuffer = mesh->GetVertexBuffer();
		ID3D11Buffer* indexBuffer = mesh->GetIndexBuffer();
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//Loop through each entity in the list
		for (size_t i = 0; i < list.size(); i++)
		{
			//Don't draw disabled entities
			if (!list[i]->GetEnabled())
				continue;

			//Prepare the material's object specific variables
			mat->PrepareMaterialObject(list[i]);

			// Finally do the actual drawing
			//  - Do this ONCE PER OBJECT you intend to draw
			//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
			//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
			//     vertices in the currently set VERTEX BUFFER
			context->DrawIndexed(
				mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
				0,     // Offset to the first index we want to use
				0);    // Offset to add to each index when looking up vertices
		}
	}
}

// Draw debug rectangles
void Renderer::DrawDebugColliders(ID3D11DeviceContext* context, Camera* camera)
{
	//Set wireframe
	context->RSSetState(RS_wireframe);

	//Set shaders
	colDebugVS->SetShader();
	colDebugPS->SetShader();

	//Set camera data
	colDebugVS->SetMatrix4x4("projection", camera->GetProjectionMatrix());
	colDebugVS->SetMatrix4x4("view", camera->GetViewMatrix());
	colDebugVS->CopyBufferData("perFrame");

	//Loop
	for (auto const& collider : debugColliders)
	{
		// Assign collider world to VS
		colDebugVS->SetMatrix4x4("world", collider->GetWorldMatrix());
		colDebugVS->CopyBufferData("perObject");

		// Set buffers in the input assembler
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		ID3D11Buffer* vertexBuffer = cubeMesh->GetVertexBuffer();
		ID3D11Buffer* indexBuffer = cubeMesh->GetIndexBuffer();
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Draw object
		context->DrawIndexed(
			cubeMesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}
	//Clear debug collider list and reset raster state
	debugColliders.clear();
	context->RSSetState(0);
}

void Renderer::DrawSky(ID3D11DeviceContext* context, Camera* camera)
{
	//Return if we don't have a skybox
	if (!skyboxMat)
		return;

	// Set up the shaders
	skyboxMat->PrepareMaterialCombo(nullptr, camera);

	// Set buffers in the input assembler
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* vertexBuffer = cubeMesh->GetVertexBuffer();
	ID3D11Buffer* indexBuffer = cubeMesh->GetIndexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set up any new render states
	context->RSSetState(skyRasterState);
	context->OMSetDepthStencilState(skyDepthState, 0);

	// Draw
	context->DrawIndexed(cubeMesh->GetIndexCount(), 0, 0);

	// Reset states
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);
}

// Apply the post process.
void Renderer::ApplyPostProcess(ID3D11DeviceContext* context,
	ID3D11RenderTargetView* backBufferRTV,
	ID3D11DepthStencilView* depthStencilView,
	ID3D11RenderTargetView* ppRTV,
	ID3D11ShaderResourceView* ppSRV,
	ID3D11SamplerState* sampler,
	UINT width, UINT height)
{

	// Set target back to back buffer.
	context->OMSetRenderTargets(1, &backBufferRTV, 0);

	// Render a full-screen triangle using the post process vertex shader.
	fxaaVS->SetShader();

	// Send the pixel shader data.
	fxaaPS->SetShader();

	// Set $GLOBAL cbuffer data.
	fxaaPS->SetShaderResourceView("g_RenderTextureView", fxaaSRV);
	fxaaPS->SetSamplerState("g_Sampler", sampler);

	// Set UniformData cbuffer data.
	fxaaPS->SetFloat2("textureResolution", DirectX::XMFLOAT2(width, height));
	fxaaPS->CopyAllBufferData(); // Copy data to shader.

	// Set FXAASettings cbuffer data.
	fxaaPS->SetFloat("FXAA_EDGE_THRESHOLD", fxaaSettings->EDGE_THRESHOLD);
	fxaaPS->SetFloat("FXAA_EDGE_THRESHOLD_MIN", fxaaSettings->EDGE_THRESHOLD_MIN);
	fxaaPS->SetFloat("FXAA_SEARCH_THRESHOLD", fxaaSettings->SEARCH_THRESHOLD);
	fxaaPS->SetFloat("FXAA_SUBPIX_CAP", fxaaSettings->SUBPIX_CAP);
	fxaaPS->SetFloat("FXAA_SUBPIX_TRIM", fxaaSettings->SUBPIX_TRIM);
	fxaaPS->SetFloat("FXAA_DEBUG_GRAYSCALE", fxaaSettings->DEBUG_GRAYSCALE);

	fxaaPS->SetInt("FXAA_ENABLED", fxaaSettings->FXAA);
	fxaaPS->SetInt("FXAA_SEARCH_STEPS", fxaaSettings->SEARCH_STEPS);
	fxaaPS->SetInt("FXAA_SEARCH_ACCELERATION", fxaaSettings->SEARCH_ACCELERATION);
	fxaaPS->SetInt("FXAA_SUBPIX", fxaaSettings->SUBPIX);
	fxaaPS->SetInt("FXAA_SUBPIX_FASTER", fxaaSettings->SUBPIX_FASTER);
	fxaaPS->SetInt("FXAA_LUMINANCE_METHOD", fxaaSettings->LUMINANCE_METHOD);

	fxaaPS->SetInt("FXAA_DEBUG_DISCARD", fxaaSettings->DEBUG_DISCARD);
	fxaaPS->SetInt("FXAA_DEBUG_PASSTHROUGH", fxaaSettings->DEBUG_PASSTHROUGH);
	fxaaPS->SetInt("FXAA_DEBUG_HORZVERT", fxaaSettings->DEBUG_HORZVERT);
	fxaaPS->SetInt("FXAA_DEBUG_PAIR", fxaaSettings->DEBUG_PAIR);
	fxaaPS->SetInt("FXAA_DEBUG_NEGPOS", fxaaSettings->DEBUG_NEGPOS);
	fxaaPS->SetInt("FXAA_DEBUG_OFFSET", fxaaSettings->DEBUG_OFFSET);
	fxaaPS->SetInt("FXAA_DEBUG_HIGHLIGHT", fxaaSettings->DEBUG_HIGHLIGHT);
	fxaaPS->SetInt("FXAA_DEBUG_GRAYSCALE_CHANNEL", fxaaSettings->DEBUG_GRAYSCALE_CHANNEL);

	fxaaPS->CopyAllBufferData(); // Copy data to shader.

	// Deactivate vertex and index buffers.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* nothing = 0;
	context->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	// Draw a set number of vertices.
	context->Draw(3, 0);

	// Unbind all pixel shader SRVs.
	ID3D11ShaderResourceView* nullSRVs[16] = {};
	context->PSSetShaderResources(0, 16, nullSRVs);

	// Reset depth stencil view.
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
}

// Add an entity to the render list
void Renderer::AddEntityToRenderer(Entity* e)
{
	//Get identifier
	std::string identifier = e->GetMatMeshIdentifier();

	//Check if we already have the mat/mesh combo in the map
	if (renderMap.find(identifier) != renderMap.end())
	{
		//Get render list
		std::vector<Entity*> list = renderMap[identifier];

		//Check the iterator of the entity
		if (std::find(list.begin(), list.end(), e) != list.end())
		{
			printf("Cannot add entity %s because it is already in renderer", e->GetName().c_str());
			return;
		}

		//Add to the list
		renderMap[identifier].push_back(e);
	}
	//else make a new entry
	else
	{
		std::vector<Entity*> list;
		list.push_back(e);
		renderMap.emplace(identifier, list);
	}
}

// Remove an entity from the render list
void Renderer::RemoveEntityFromRenderer(Entity* e)
{
	//Get iterator
	std::string identifier = e->GetMatMeshIdentifier();
	auto mapIt = renderMap.find(identifier);

	//Check if we are in the map
	if (mapIt == renderMap.end())
	{
		printf("Cannot remove entity because it is not in renderer");
		return;
	}

	//Get correct render list
	std::vector<Entity*>* list = &renderMap[identifier];

	//Get the iterator of the entity
	std::vector<Entity*>::iterator listIt = std::find((*list).begin(), (*list).end(), e);

	//Check if we are in the list
	if (listIt == (*list).end())
	{
		printf("Cannot remove entity because it is not in renderer");
		return;
	}

	//Swap it for the last one
	std::swap(*listIt, (*list).back());

	//Pop the last one
	(*list).pop_back();

	//Check if the list is empty
	if ((*list).size() == 0)
	{
		//Erase
		renderMap.erase(e->GetMatMeshIdentifier());
	}
}

// Check if an entity is in the render list. O(n) complexity
//	(n is the amount of entities that share the material and mesh)
bool Renderer::IsEntityInRenderer(Entity* e)
{
	//Early return if render list is empty and if the entity is in it
	std::string identifier = e->GetMatMeshIdentifier();
	if (renderMap.find(identifier) == renderMap.end())
		return false;

	//Get render list
	std::vector<Entity*> list = renderMap[identifier];

	//Early return if render list is empty
	if (list.size() == 0)
		return false;

	//Check the iterator of the entity
	if (std::find(list.begin(), list.end(), e) == list.end())
		return false;

	return true;
}

// Tell the renderer to render a collider this frame
void Renderer::AddDebugColliderToThisFrame(Collider * c)
{
	debugColliders.push_back(c);
}

// Set the clear color.
void Renderer::SetClearColor(const float color[4])
{
	for (int i = 0; i < 4; i++)
	{
		clearColor[i] = color[i];
	}
}

// Set the clear color.
void Renderer::SetClearColor(float r, float g, float b, float a)
{
	clearColor[0] = r;
	clearColor[1] = g;
	clearColor[2] = b;
	clearColor[3] = a;
}