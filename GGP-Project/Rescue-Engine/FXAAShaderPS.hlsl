
// Data to pull from the constant buffer.
cbuffer FXAAData : register(b0)
{
	float texelWidth;
	float texelHeight;
	int blurAmount;
}

//  Define input to this shader.
struct VertexToPixel
{
	float4 position			: SV_POSITION;
	float2 uv				: TEXCOORD0;
};

// Textures to be added.
Texture2D pixels			: register(t0);
SamplerState basicSampler	: register(s0);

// Entry point for the pixel shader.
float4 main(VertexToPixel input) : SV_TARGET
{

	// Placeholder blur.

	// Track the total color and samples
	float4 totalColor = float4(0,0,0,0);
	uint numSamples = 0;

	// Loop and sample surrounding pixels
	// A "box blur", if you will
	for (int y = -blurAmount; y <= blurAmount; y += 1)
	{
		for (int x = -blurAmount; x <= blurAmount; x += 1)
		{
			float2 uv = input.uv + float2(x * texelWidth, y * texelWidth);
			totalColor += pixels.Sample(basicSampler, uv);

			numSamples++;
		}
	}

	return totalColor / numSamples;
}