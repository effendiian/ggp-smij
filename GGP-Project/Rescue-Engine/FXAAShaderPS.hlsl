
// Constant buffers should be organized by the frequency of update.

// Data to pull from the constant buffer.
cbuffer UniformData : register(b0)
{
	// State data.
	float2 textureResolution;
	float grayscalePercentage;

}

// FXAA calculations that must be done.
cbuffer FXAASettings				: register(b1)
{
	// FXAA flags.
	float FXAA_EDGE_THRESHOLD;
	float FXAA_EDGE_THRESHOLD_MINIMUM;
	float FXAA_SEARCH_THRESHOLD;
	float FXAA_SUBPIX_CAP;
	float FXAA_SUBPIX_TRIM;
	uint FXAA_ENABLED;
	uint FXAA_SEARCH_STEPS;
	uint FXAA_SEARCH_ACCELERATION;
	uint FXAA_SUBPIX;
	uint FXAA_SUBPIX_FASTER;
	uint FXAA_LUMINANCE_METHOD;

	// FXAA debug flags.
	uint FXAA_DEBUG_DISCARD;
	uint FXAA_DEBUG_PASSTHROUGH;
	uint FXAA_DEBUG_HORZVERT;
	uint FXAA_DEBUG_PAIR;
	uint FXAA_DEBUG_NEGPOS;
	uint FXAA_DEBUG_OFFSET;
	uint FXAA_DEBUG_HIGHLIGHT;
}

// $Global input values.
Texture2D g_RenderTextureView			: register(t0);
SamplerState g_Sampler					: register(s0);

//  Define input to this shader.
struct VertexToPixel
{
	float4 position					: SV_POSITION;
	float2 uv						: TEXCOORD0;
};

// Entry point for the pixel shader.
float4 main(VertexToPixel input) : SV_TARGET {

	// Track the total color and samples
	float4 totalColor = float4(0,0,0,0);
	uint numSamples = 0;

	float texelWidth = textureResolution.x;
	float texelHeight = textureResolution.y;

	// Loop and sample surrounding pixels
	// A "box blur", if you will
	for (int y = -2; y <= 2; y += 1)
	{
		for (int x = -2; x <= 2; x += 1)
		{
			float2 uv = input.uv + float2(x * texelWidth, y * texelHeight);
			totalColor += g_RenderTextureView.Sample(g_Sampler, uv);

			numSamples++;
		}
	}

	return totalColor / numSamples;
}