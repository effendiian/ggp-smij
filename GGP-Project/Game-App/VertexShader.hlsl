
//Data that changes once per MatMesh combo
cbuffer perCombo : register(b0)
{
	matrix view;
	matrix projection;
	float2 uvScale;
	matrix shadowView;
	matrix shadowProj;
}

//Data that changes once per MatMesh combo
cbuffer perObject : register(b1)
{
	matrix world;
	matrix worldInvTrans;
}

// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
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
// - Should match our pixel shader's input (hence the name: Vertex to Pixel)
// - At a minimum, we need a piece of data defined tagged as SV_POSITION
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;	 // XYZW position (System Value Position)
	float2 uv			: TEXCOORD;		 // XY uv
	float3 normal		: NORMAL;        // XYZ normal
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;		 // world position of the vertex
	float4 posForShadow : SHADOW;
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
VertexToPixel main(VertexShaderInput input )
{
	// Set up output struct
	VertexToPixel output;

	// The vertex's position (input.position) must be converted to world space,
	// then camera space (relative to our 3D camera), then to proper homogenous 
	// screen-space coordinates.  This is taken care of by our world, view and
	// projection matrices.  
	//
	// First we multiply them together to get a single matrix which represents
	// all of those transformations (world to view to projection space)
	matrix worldViewProj = mul(mul(world, view), projection);

	// Calculate shadow map position
	matrix shadowWVP = mul(mul(world, shadowView), shadowProj);
	output.posForShadow = mul(float4(input.position, 1.0f), shadowWVP);

	// Then we convert our 3-component position vector to a 4-component vector
	// and multiply it by our final 4x4 matrix.
	//
	// The result is essentially the position (XY) of the vertex on our 2D 
	// screen and the distance (Z) from the camera (the "depth" of the pixel)
	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.worldPos = mul(float4(input.position, 1.0f), world).xyz;
	output.normal = normalize(mul(input.normal, (float3x3)worldInvTrans));
	output.tangent = normalize(mul(input.tangent, (float3x3)worldInvTrans));
	output.uv = input.uv * uvScale;

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}