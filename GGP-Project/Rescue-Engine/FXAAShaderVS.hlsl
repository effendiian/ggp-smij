
// No input. Vertex shader is drawing a full-screen triangle. (3 vertices).

// Defines the output data of the vertex shader.
struct VertexToPixel
{
	float4 position			: SV_POSITION;
	float2 uv				: TEXCOORD0;
};

// The entry point for the vertex shader.
VertexToPixel main(uint id : SV_VertexID)
{
	// Set up output.
	VertexToPixel output;

	// Calculate the UV (0,0) to (2,2) via the ID.
	output.uv = float2((id << 1) & 2, id & 2);

	// Adjust the position based on the UV.
	output.position = float4(output.uv, 0, 1);
	output.position.x = output.position.x * 2 - 1;
	output.position.y = output.position.y * -2 + 1;

	// Return the output.
	return output;
}