#include "Renderer.h"
#include "LightManager.h"

using namespace DirectX;

// Initialize values in the renderer
void Renderer::Init()
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;

	//Set ambient light
	LightManager* lightManager = LightManager::GetInstance();
	lightManager->SetAmbientColor(0.01f, 0.01f, 0.01f);

	//Directional lights
	lightManager->CreateDirectionalLight(XMFLOAT3(1, 1, 1), 1);

	//Point light
	pLight = lightManager->CreatePointLight(5, XMFLOAT3(0, 1, 0), 1);
	pLight->SetPosition(0, -2, 3);

	//Spot light
	pLight2 = lightManager->CreatePointLight(5, XMFLOAT3(0, 0, 1), 1);
	pLight2->SetPosition(2, 0, -1);
}

// Destructor for when the singleton instance is deleted
Renderer::~Renderer()
{ }

// Draw all entities in the render list
void Renderer::Draw(ID3D11DeviceContext* context, Camera* camera)
{
	//TODO: Apply attenuation
	for (auto const& x : renderMap)
	{
		if (x.second.size() < 1)
			return;

		//Get list, material, and mesh
		std::vector<Entity*> list = x.second;
		Material* mat = list[0]->GetMaterial();
		Mesh* mesh = list[0]->GetMesh();

		//Prepare the material's combo specific variables
		mat->PrepareMaterialCombo(list[0], camera);

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

// Add an entity to the render list
void Renderer::AddEntityToRenderList(Entity* e)
{
	bool isInList = IsEntityInRenderList(e);
	if (IsEntityInRenderList(e))
	{
		printf("Cannot add entity because it is already in render list");
		return;
	}

	//Look for existing mesh/material combos
	if (renderMap.find(e->GetMatMeshIdentifier()) != renderMap.end())
	{
		renderMap[e->GetMatMeshIdentifier()].push_back(e);
	}
	//Make a new entry
	else
	{
		std::vector<Entity*> list;
		list.push_back(e);
		renderMap.emplace(e->GetMatMeshIdentifier(), list);
	}
}

// Remove an entity from the render list
void Renderer::RemoveEntityFromRenderList(Entity* e)
{
	//Early return if render list is empty and if the entity is in it
	if (renderMap.find(e->GetMatMeshIdentifier()) == renderMap.end())
		return;

	//Get correct render list
	std::vector<Entity*> list = renderMap[e->GetMatMeshIdentifier()];

	//Get the index of the entity
	size_t* index = nullptr;
	size_t i;
	for (i = 0; i < list.size(); i++)
	{
		if (e == list[i])
		{
			index = &i;
			break;
		}
	}

	//Entity is not in the render list
	if (index == nullptr)
	{
		printf("Cannont remove entity because it is not in render list");
		return;
	}

	//If the entity is not the very last we swap it for the last one
	if (*index != list.size() - 1)
	{
		std::swap(list[*index], list[list.size() - 1]);
	}

	//Pop the last one
	list.pop_back();

	//Check if the list is empty
	if (list.size() == 0)
	{
		//Erase
		renderMap.erase(e->GetMatMeshIdentifier());
	}
}

// Check if an entity is in the render list. O(n) complexity
//	(n is the amount of entities that share the material and mesh)
bool Renderer::IsEntityInRenderList(Entity* e)
{
	//Early return if render list is empty and if the entity is in it
	if (renderMap.find(e->GetMatMeshIdentifier()) == renderMap.end())
		return false;

	//Get render list
	std::vector<Entity*> list = renderMap[e->GetMatMeshIdentifier()];

	//Early return if render list is empty
	if (list.size() == 0)
		return false;

	//Get the index of the entity
	for (size_t i = 0; i < list.size(); i++)
	{
		if (e == list[i])
		{
			return true;
		}
	}

	return false;
}