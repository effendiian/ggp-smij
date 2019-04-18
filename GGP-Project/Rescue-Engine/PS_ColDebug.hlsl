
//Pixel shader for when drawing debug collider outlines

// Struct representing the data we expect to receive from earlier pipeline stages
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 worldPos		: POSITION;		 // world position of the vertex
	float2 uv			: TEXCOORD;		 // XY uv
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	//Return final pixel color
	return float4(1, 0, 0, 1);
}