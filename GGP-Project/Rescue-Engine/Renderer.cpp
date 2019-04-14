#include "Renderer.h"
#include "LightManager.h"
#include <algorithm>

using namespace DirectX;

// Initialize values in the renderer
void Renderer::Init()
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;
}

// Destructor for when the singleton instance is deleted
Renderer::~Renderer()
{ }

// Draw all entities in the render list
void Renderer::Draw(ID3D11DeviceContext* context, Camera* camera)
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

	//for()

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

void Renderer::AddBoxToRenderer()
{
	//resourceManager->GetMesh("Assets\\Models\\cube.obj");
}
