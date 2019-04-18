/*****************************************
 * FXAA Pixel Shader - effendiian
 * -------------------------------------
 * FXAA implementation based off of the
 * work by Timothy Lottes in the Nvidia white paper:
 * https://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf
 *
 * Also used these resources:
 * - https://catlikecoding.com/unity/tutorials/advanced-rendering/fxaa/
 * - https://blog.codinghorror.com/fast-approximate-anti-aliasing-fxaa/
 *****************************************/

// ------------------------------------- //

// Constant buffers should be organized by the frequency of update.

// Data to pull from the constant buffer.
cbuffer UniformData : register(b0)
{
	// State data.
	float2 textureResolution;

}

// FXAA calculations that must be done.
cbuffer FXAASettings				: register(b1)
{
	// FXAA flags.
	float FXAA_EDGE_THRESHOLD;
	float FXAA_EDGE_THRESHOLD_MIN;
	float FXAA_SEARCH_THRESHOLD;
	float FXAA_SUBPIX_CAP;
	float FXAA_SUBPIX_TRIM;
	float FXAA_DEBUG_GRAYSCALE;
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
	uint FXAA_DEBUG_GRAYSCALE_CHANNEL;
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

// --------------------------------------
// Helper functions.
// --------------------------------------

// ---------------------
// Conversion functions.

// ToFloat2
float2 ToFloat2(float value) { return float2(value, value); }

// ToFloat3
float3 ToFloat3(float value) { return float3(value, value, value); }
float3 ToFloat3(float2 value, float z) { return float3(value.x, value.y, z); }
float3 ToFloat3(float2 value) { return ToFloat3(value, 0.0); }

// ToFloat4
float4 ToFloat4(float2 value, float z, float w) { return float4(value.x, value.y, z, w); }
float4 ToFloat4(float2 value, float z) { return ToFloat4(value, z, 0.0); }
float4 ToFloat4(float2 value) { return ToFloat4(value, 0.0); }
float4 ToFloat4(float3 value, float w) { return float4(value.x, value.y, value.z, w); }
float4 ToFloat4(float3 value) { return ToFloat4(value, 0.0); }
float4 ToFloat4(float value, float w) { return float4(value, value, value, w); }
float4 ToFloat4(float value) { return ToFloat4(value, 0.0); }

// ---------------------
// Texture sampler functions.

// Sample point given origin and offset value.
float4 SampleOffset( Texture2D tex,
							float2 uv,
							float2 offset )
{

	// Return point from specified location.
	return tex.Sample(g_Sampler, uv + offset);

}

// ---------------------
// Grayscale functions.

// Return grayscaled image based off of the selected color channel.
float3 Grayscale(float3 color, int index)
{
	int selectedChannel = clamp(index, 0, 2); // [0]r, [1]g, [2]b.
	return ToFloat3(color[selectedChannel]);
}

// Return grayscaled image based off of the selected color channel.
float4 Grayscale(float4 color, int index)
{
	int selectedChannel = clamp(index, 0, 3); // [0]r, [1]g, [2]b, [3]a.
	return ToFloat4(color[selectedChannel]);
}

// Default to green color channel when no index is supplied.
float3 Grayscale(float3 color) { return Grayscale(color, 1); }
float4 Grayscale(float4 color) { return Grayscale(color, 1); }

// ---------------------
// Luminance functions.

// Luma is input with color as the alpha value. No calculations.
float LuminanceRGBL(float4 color)
{
	return color.a;
}

// Luma calculated using function from the original FXAA source specification.
float LuminanceOriginal(float4 color)
{
	float3 weight = float3(0.299, 0.587, 0.114);
	return dot(color.rgb, weight);
}

// Luma calculated using function from the revised FXAA 3.11 source specification.
float LuminanceRevised(float4 color)
{
	return color.g * (0.587 / 0.299) + color.r;
}

// Luma calculated using standard weights for relative luma from https://en.wikipedia.org/wiki/Luma_(video).
float LuminanceLinear(float4 color)
{
	float3 weight = float3(0.2126729, 0.7151522, 0.0721750);
	return dot(color.rgb, weight);
}

// Luma used is equivalent to the raw red. (Rare).
float LuminanceRed(float4 color)
{
	return color.r;
}

// Luma used is approximate to the gamma-correct red. (Rare).
float LuminanceRedGamma(float4 color)
{
	return sqrt(LuminanceRed(color));
}

// Luma used is equivalent to the raw green. (Common).
float LuminanceGreen(float4 color)
{
	return color.g;
}

// Luma used is approximate to the gamma-correct green. (Common).
float LuminanceGreenGamma(float4 color)
{
	return sqrt(LuminanceGreen(color));
}

// Luma used is equivalent to the raw blue. (Rare).
float LuminanceBlue(float4 color)
{
	return color.b;
}

// Luma used is approximate to the gamma-correct blue. (Rare).
float LuminanceBlueGamma(float4 color)
{
	return sqrt(LuminanceBlue(color));
}

// Calculate luminance using specified luminance method. Defaults to revised.
float Luminance(float4 color)
{
	if (FXAA_LUMINANCE_METHOD == 0) { return LuminanceRGBL(color); }
	if (FXAA_LUMINANCE_METHOD == 1) { return LuminanceOriginal(color); }
	if (FXAA_LUMINANCE_METHOD == 2) { return LuminanceRevised(color); }
	if (FXAA_LUMINANCE_METHOD == 3) { return LuminanceLinear(color); }
	if (FXAA_LUMINANCE_METHOD == 4) { return LuminanceRed(color); }
	if (FXAA_LUMINANCE_METHOD == 5) { return LuminanceRedGamma(color); }
	if (FXAA_LUMINANCE_METHOD == 6) { return LuminanceGreen(color); }
	if (FXAA_LUMINANCE_METHOD == 7) { return LuminanceGreenGamma(color); }
	if (FXAA_LUMINANCE_METHOD == 8) { return LuminanceBlue(color); }
	if (FXAA_LUMINANCE_METHOD == 9) { return LuminanceBlueGamma(color); }

	// If no method detected or invalid setting, use default REVISED.
	return LuminanceRevised(color);
}

// Handle calls with three component float.
float Luminance(float3 color) { return Luminance(float4(color, 1.0)); }

// ------------------------
// FXAA specific functions.
// ------------------------

// ---------------------
// Vertical/Horizontal Edge Test functions.

// Calculate luma value for vertical edge for check.
float FXAAVerticalEdge(float lumaO,
	float lumaN,
	float lumaE,
	float lumaS,
	float lumaW,
	float lumaNW,
	float lumaNE,
	float lumaSW,
	float lumaSE)
{

	// Slices to calculate.
	float top = (0.25 * lumaNW) + (-0.5 * lumaN) + (0.25 * lumaNE);
	float middle = (0.50 * lumaW) + (-1.0 * lumaO) + (0.50 * lumaE);
	float bottom = (0.25 * lumaSW) + (-0.5 * lumaS) + (0.25 * lumaSE);

	// Return value.
	return abs(top) + abs(middle) + abs(bottom);
}

// Calculate luma value for horizontal edge for check.
float FXAAHorizontalEdge(float lumaO,
	float lumaN,
	float lumaE,
	float lumaS,
	float lumaW,
	float lumaNW,
	float lumaNE,
	float lumaSW,
	float lumaSE)
{

	// Slices to calculate.
	float top = (0.25 * lumaNW) + (-0.5 * lumaW) + (0.25 * lumaSW);
	float middle = (0.50 * lumaN) + (-1.0 * lumaO) + (0.50 * lumaS);
	float bottom = (0.25 * lumaNE) + (-0.5 * lumaE) + (0.25 * lumaSE);

	// Return value.
	return abs(top) + abs(middle) + abs(bottom);
}

// Wrapper function.
float FXAAEdge(bool isHorizontal,
			   float lumaO,
			   float lumaN,
			   float lumaE,
			   float lumaS,
			   float lumaW,
			   float lumaNW,
			   float lumaNE,
			   float lumaSW,
			   float lumaSE)
{
	return (isHorizontal)
		? FXAAHorizontalEdge(lumaO, lumaN, lumaE, lumaS, lumaW, lumaNW, lumaNE, lumaSW, lumaSE)
		: FXAAVerticalEdge(lumaO, lumaN, lumaE, lumaS, lumaW, lumaNW, lumaNE, lumaSW, lumaSE);
}

// ---------------------
// FXAA Algorithim.

// Entry point for FXAA process.
float3 ApplyFXAA( float2 uv )
{

	// Typically this would also take a texture,
	// dimensions, and the screen resolution,
	// but we don't need to do that in this instance;
	// we already have a normalized uv that's input
	// from the vertex shader and some of our other
	// values are exposed through the constant buffer.

	// Already sent from the Vertex Shader.
	// float2 uv = pixelPosition / screenResolution;

	// Distance between pixels in texture space.
	float2 texel = float2(1, 1) / textureResolution;

	//-------------------------
	// 1. LOCAL CONTRAST CHECK

	// Sample textures from cardinal directions.
	float3 rgbN = SampleOffset(g_RenderTextureView, uv, float2(0, -texel.y)).rgb; // NORTH
	float3 rgbW = SampleOffset(g_RenderTextureView, uv, float2(-texel.x, 0)).rgb; // WEST
	float3 rgbO = SampleOffset(g_RenderTextureView, uv, float2(0, 0)).rgb; // ORIGIN
	float3 rgbE = SampleOffset(g_RenderTextureView, uv, float2(texel.x, 0)).rgb; // EAST
	float3 rgbS = SampleOffset(g_RenderTextureView, uv, float2(0, texel.y)).rgb; // SOUTH

	// Calculate the luminance for each sampled value.
	float lumaN = Luminance(rgbN);
	float lumaW = Luminance(rgbW);
	float lumaO = Luminance(rgbO);
	float lumaE = Luminance(rgbE);
	float lumaS = Luminance(rgbS);

	// Calculate the minimum luma range.
	float minLuma = min(lumaO, min(min(lumaN, lumaW), min(lumaS, lumaE)));
	float maxLuma = max(lumaO, max(max(lumaN, lumaW), max(lumaS, lumaE)));
	float localContrast = maxLuma - minLuma;

	// Check for early exit.
	if (localContrast < max(FXAA_EDGE_THRESHOLD_MIN, maxLuma * FXAA_EDGE_THRESHOLD)) {

		// Return a black pixel if we are discarding; otherwise, return an unaltered pixel.
		if (FXAA_DEBUG_DISCARD == 1) { return ToFloat3(0); }
		return rgbO; // Unaltered pixel.
	}

	// Default return value.
	return rgbO;

}

// Entry point for the pixel shader.
float4 main(VertexToPixel input) : SV_TARGET
{
	// Variable to store pixel color.
	float4 pixelColor = float4(0, 0, 0, 0);

	// If the FXAA process is not enabled...
	if (FXAA_ENABLED == 0)
	{
		// Sample from the RTV and exit early.
		return g_RenderTextureView.Sample(g_Sampler, input.uv);
	}

	// Apply FXAA.
	pixelColor = ToFloat4(ApplyFXAA(input.uv), 1.0);

	// Apply grayscale from debug flag.
	pixelColor = lerp(pixelColor, Grayscale(pixelColor, FXAA_DEBUG_GRAYSCALE_CHANNEL), FXAA_DEBUG_GRAYSCALE);

	// Output the pixel color.
	return pixelColor;
}