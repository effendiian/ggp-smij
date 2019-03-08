#include "Renderer.h"

using namespace DirectX;

// Initialize values in the renderer
void Renderer::Init()
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;

	//Directional lights
	dLight = new DirectionalLight(XMFLOAT4(0.1f, 0.1f, 0.1f, 1), XMFLOAT4(1, 1, 1, 1), 1);

	//Point light
	pLight = new PointLight(5, XMFLOAT4(0, 1, 0, 1), 1);
	pLight->SetPosition(0, -2, 3);

	//Spot light
	sLight = new SpotLight(5, XMFLOAT4(0, 0, 1, 1), 1);
	sLight->SetPosition(2, 0, -1);
	sLight->SetRotation(0, -90, 0);
}

// Destructor for when the singleton instance is deleted
Renderer::~Renderer()
{ 
	//Release lights
	if (dLight)
		delete dLight;
	if (pLight)
		delete pLight;
	if (sLight)
		delete sLight;
}

// Draw all entities in the render list
void Renderer::Draw(ID3D11DeviceContext* context, Camera* camera)
{
	//TODO: Sort enties based on meshes/materials
	//TODO: Assign lights to entities
	//TODO: Apply attenuation
	for (size_t i = 0; i < renderList.size(); i++)
	{
		Material* mat = renderList[i]->GetMaterial();

		//Get the entity's shaders
		vertexShader = renderList[i]->GetVertexShader();
		pixelShader = renderList[i]->GetPixelShader();

		// Send data to shader variables
		//  - Do this ONCE PER OBJECT you're drawing
		//  - This is actually a complex process of copying data to a local buffer
		//    and then copying that entire buffer to the GPU.  
		//  - The "SimpleShader" class handles all of that for you.
		vertexShader->SetMatrix4x4("world", renderList[i]->GetWorldMatrix());
		vertexShader->SetMatrix4x4("view", camera->GetViewMatrix());
		vertexShader->SetMatrix4x4("projection", camera->GetProjectionMatrix());
		vertexShader->SetMatrix4x4("worldInvTrans", renderList[i]->GetWorldInvTransMatrix());
	
		pixelShader->SetData("light1", dLight->GetLightStruct(), sizeof(LightStruct));
		pixelShader->SetData("light2", pLight->GetLightStruct(), sizeof(LightStruct));
		pixelShader->SetData("light3", sLight->GetLightStruct(), sizeof(LightStruct));
		pixelShader->SetFloat4("surfaceColor", mat->GetSurfaceColor());
		pixelShader->SetFloat3("cameraPosition", camera->GetPosition());
		pixelShader->SetFloat("specularity", mat->GetSpecularity());
		pixelShader->SetShaderResourceView("diffuseTexture", mat->GetResourceView());
		pixelShader->SetSamplerState("basicSampler", mat->GetSamplerState());

		// Once you've set all of the data you care to change for
		// the next draw call, you need to actually send it to the GPU
		//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
		vertexShader->CopyAllBufferData();
		pixelShader->CopyAllBufferData();

		// Set the vertex and pixel shaders to use for the next Draw() command
		//  - These don't technically need to be set every frame...YET
		//  - Once you start applying different shaders to different objects,
		//    you'll need to swap the current shaders before each draw
		vertexShader->SetShader();
		pixelShader->SetShader();

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		ID3D11Buffer* vertexBuffer = renderList[i]->GetVertexBuffer();
		ID3D11Buffer* indexBuffer = renderList[i]->GetIndexBuffer();
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			renderList[i]->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}
}

// Add an entity to the render list
void Renderer::AddEntityToRenderList(Entity* e)
{
	renderList.push_back(e);
}

// Remove an entity from the render list
void Renderer::RemoveEntityFromRenderList(Entity* e)
{
	//Early return if render list is empty
	if (renderList.size() == 0)
		return;

	//Get the index of the entity
	size_t* index = nullptr;
	size_t i;
	for (i = 0; i < renderList.size(); i++)
	{
		if (e == renderList[i])
		{
			index = &i;
			break;
		}
	}

	//Entity is not in the render list
	if (index == nullptr)
		return;

	//If the entity is not the very last we swap it for the last one
	if (*index != renderList.size() - 1)
	{
		std::swap(renderList[*index], renderList[renderList.size() - 1]);
	}

	//Pop the last one
	renderList.pop_back();
}

// Check if an entity is in the render list. O(n) complexity
bool Renderer::IsEntityInRenderList(Entity* e)
{
	//Early return if render list is empty
	if (renderList.size() == 0)
		return false;

	//Get the index of the entity
	for (size_t i = 0; i < renderList.size(); i++)
	{
		if (e == renderList[i])
		{
			return true;
		}
	}

	return false;
}