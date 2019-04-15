
//Vertex shader for when drawing debug collider outlines

//Data that changes once per frame
cbuffer perFrame : register(b0)
{
	matrix view;
	matrix projection;
}

//Data that changes once per object
cbuffer perObject : register(b1)
{
	matrix world;
}

// Struct representing a single vertex worth of data
struct VertexShaderInput
{ 
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 position		: POSITION;	     // XYZ position
	float2 uv			: TEXCOORD;		 // XY uv
	float3 normal		: NORMAL;        // XYZ normal
	float3 tangent		: TANGENT;
};

// Struct representing the data we're sending down the pipeline
struct VertexToPixel
{
	float4 position		: SV_POSITION;	 // XYZW position (System Value Position)
	float3 worldPos		: POSITION;		 // world position of the vertex
	float2 uv			: TEXCOORD;		 // XY uv
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixel main(VertexShaderInput input )
{
	// Set up output struct
	VertexToPixel output;

	// The vertex's position (input.position) must be converted to world space
	matrix worldViewProj = mul(mul(world, view), projection);

	// Then we convert our 3-component position vector to a 4-component vector
	// and multiply it by our final 4x4 matrix.
	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.worldPos = mul(float4(input.position, 1.0f), world).xyz;
	output.uv = input.uv;

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}