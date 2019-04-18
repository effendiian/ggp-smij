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

	//-------------------------
	// 2. SUB-PIXEL ALIASING TEST

	// Calculate the pixel contrast ratio.
	// - Sub-pixel aliasing is detected by taking the ratio of the
	// pixel contrast over the local contrast. This ratio nears 1.0
	// in the presence of single pixel dots and otherwise falls off
	// towards 0.0 as more pixels contribute to an edge. This ratio
	// is transformed into the amount of lowpass filter to blend in
	// at the end of the algorithm.

	// Declare some variables.
	float3 rgbL = 0.0;
	float lumaL = 0.0;
	float pixelContrast = 0.0;
	float contrastRatio = 0.0;
	float lowpassBlend = 0.0; // Default is zero. Will be changed depending on subpixel level.
	float FXAA_SUBPIX_TRIM_SCALE = (1.0 / (1.0 - FXAA_SUBPIX_TRIM));

	if (FXAA_SUBPIX > 0)
	{
		// Calculate sum of local samples for the lowpass.
		rgbL = (rgbN + rgbW + rgbO + rgbE + rgbS);

		if (FXAA_SUBPIX_FASTER > 0)
		{
			// Average the lowpass now since this skips the addition of the diagonal neighbors (NW, NE, SW, SE).
			rgbL *= (1.0 / 5.0);
		}

		// Calculate the lowpass luma.
		// - Lowpass luma is calculated as the average between the luma of neigboring pixels.
		lumaL = (lumaN + lumaW + lumaS + lumaE) * 0.25;

		// Calculate the pixel contrast.
		// - Pixel contrast is the abs() difference between origin pixel luma and lowpass luma of neighbors.
		pixelContrast = abs(lumaL - lumaO);

		// Calculate the ratio between the pixelContrast and localContrast.
		contrastRatio = pixelContrast / localContrast;

		if (FXAA_SUBPIX == 1)
		{
			// Normal subpixel aliasing. Set based on FXAA algorithm for subpixel aliasing.
			lowpassBlend = max(0.0, contrastRatio - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE;
			lowpassBlend = min(FXAA_SUBPIX_CAP, lowpassBlend);
		}

		if (FXAA_SUBPIX == 2)
		{
			// Full force subpixel aliasing. Set blend to ratio.
			lowpassBlend = contrastRatio;
		}
	}

	// Show selected pixels if debug mode is active.
	if (FXAA_DEBUG_PASSTHROUGH > 0)
	{
		if (FXAA_SUBPIX > 0) { return float3(localContrast, lowpassBlend, 0.0); }
		return float3(localContrast, 0.0, 0.0);
	}

	//-------------------------
	// 3. VERTICAL & HORIZONTAL EDGE TEST

	// Sample the additional diagonal neighbors.
	float3 rgbNW = SampleOffset(g_RenderTextureView, uv, float2(-texel.x, -texel.y)).rgb; // NORTH-WEST
	float3 rgbNE = SampleOffset(g_RenderTextureView, uv, float2(texel.x, -texel.y)).rgb; // NORTH-EAST
	float3 rgbSW = SampleOffset(g_RenderTextureView, uv, float2(-texel.x, texel.y)).rgb; // SOUTH-WEST
	float3 rgbSE = SampleOffset(g_RenderTextureView, uv, float2(texel.x, texel.y)).rgb; // SOUTH-EAST

	// Average additional neighbors when sub-pix aliasing is on and it isn't in 'fast' mode.
	if (FXAA_SUBPIX > 0)
	{
		if (FXAA_SUBPIX_FASTER == 0)
		{
			// Add missing neighbors and average them.
			rgbL += (rgbNW + rgbNE + rgbSW + rgbSE);
			rgbL *= (1.0 / 9.0);
		}
	}

	// Calculate luma for additional neighbors.
	float lumaNW = Luminance(rgbNW);
	float lumaNE = Luminance(rgbNE);
	float lumaSW = Luminance(rgbSW);
	float lumaSE = Luminance(rgbSE);

	// Calculate the vertical and horizontal edges. (Uses algorithm from FXAA white paper).
	float edgeHori = FXAAEdge(true, lumaO, lumaN, lumaE, lumaS, lumaW, lumaNW, lumaNE, lumaSW, lumaSE);
	float edgeVert = FXAAEdge(false, lumaO, lumaN, lumaE, lumaS, lumaW, lumaNW, lumaNE, lumaSW, lumaSE);

	// Check if edge is horizontal.
	bool isHorizontal = edgeHori >= edgeVert;

	// Display horizontal/vertical edge detection results if respective debug mode is on.
	if (FXAA_DEBUG_HORZVERT > 0)
	{
		float3 gold = float3(1.0, 0.75, 0.0);
		float3 blue = float3(0.10, 0.10, 1.0);
		return (isHorizontal) ? gold : blue;
	}

	//-------------------------
	// 4. FIND HIGHEST CONTRAST PAIR 90deg TO EDGE

	// Contain the appropriate sign for the top left.
	float edgeSign = isHorizontal ? -texel.y : -texel.x; // Note, if isHorizontal == true, -texel.y is applied (not -texel.x).

	// Calculate the gradients. The luma used changes based on the horizontal edge status.
	float gradientNeg = isHorizontal ? abs(lumaN - lumaO) : abs(lumaW - lumaO);
	float gradientPos = isHorizontal ? abs(lumaS - lumaO) : abs(lumaE - lumaO);

	// Calculate the luma based on its direction.
	// It is an average of the origin and the luma in the respective direction.
	float lumaNeg = isHorizontal ? ((lumaN + lumaO) * 0.5) : ((lumaW + lumaO) * 0.5);
	float lumaPos = isHorizontal ? ((lumaS + lumaO) * 0.5) : ((lumaE + lumaO) * 0.5);

	// Select the highest gradient pair.
	bool isNegative = (gradientNeg >= gradientPos);
	float gradientHighest = isNegative ? gradientNeg : gradientPos; // Assign higher pair.
	float lumaHighest = isNegative ? lumaNeg : lumaPos;

	// If gradient pair in the negative direction is higher, flip the edge sign.
	if (isNegative) { edgeSign *= -1.0; }

	// Display detected pairs if debug mode is on.
	if (FXAA_DEBUG_PAIR > 0)
	{
		return isHorizontal ? float3(0.0, gradientHighest, lumaHighest) : float3(0.0, lumaHighest, gradientHighest);
	}

	//-------------------------
	// 5. END-OF-EDGE SEARCH


	// Select starting point.
	float2 pointN = ToFloat2(0.0);
	pointN.x = uv.x + (isHorizontal ? 0.0 : edgeSign * 0.5);
	pointN.y = uv.y + (isHorizontal ? edgeSign * 0.5 : 0.0);

	// Assign search limiting values.
	gradientHighest *= FXAA_SEARCH_THRESHOLD;

	// Prepare variables for search.
	float2 pointP = pointN; // Start at the same point.
	float2 pointOffset = isHorizontal ? float2(texel.x, 0.0) : float2(0.0, texel.y);
	float lumaNegEnd = lumaNeg;
	float lumaPosEnd = lumaPos;
	bool searchNeg = false;
	bool searchPos = false;

	// Apply values based on FXAA flags.
	if (FXAA_SEARCH_ACCELERATION == 1) {

		pointN += pointOffset * ToFloat2(-1.0);
		pointP += pointOffset * ToFloat2(1.0);
		// pointOffset *= ToFloat2(1.0);

	}
	else if (FXAA_SEARCH_ACCELERATION == 2) {

		pointN += pointOffset * ToFloat2(-1.5);
		pointP += pointOffset * ToFloat2(1.5);
		pointOffset *= ToFloat2(2.0);

	}
	else if (FXAA_SEARCH_ACCELERATION == 3) {

		pointN += pointOffset * ToFloat2(-2.0);
		pointP += pointOffset * ToFloat2(2.0);
		pointOffset *= ToFloat2(3.0);

	}
	else if (FXAA_SEARCH_ACCELERATION == 4) {

		pointN += pointOffset * ToFloat2(-2.5);
		pointP += pointOffset * ToFloat2(2.5);
		pointOffset *= ToFloat2(4.0);

	}

	// Perform the end-of-edge search.
	for (unsigned int i = 0; i < FXAA_SEARCH_STEPS; i++)
	{
		if (FXAA_SEARCH_ACCELERATION == 1) {
			if (!searchNeg) { lumaNegEnd = Luminance(g_RenderTextureView.SampleLevel(g_Sampler, pointN, 0).rgb); }
			if (!searchPos) { lumaPosEnd = Luminance(g_RenderTextureView.SampleLevel(g_Sampler, pointP, 0).rgb); }
		}
		else
		{
			if (!searchNeg) { lumaNegEnd = Luminance(g_RenderTextureView.SampleGrad(g_Sampler, pointN, pointOffset, pointOffset).rgb); }
			if (!searchPos) { lumaPosEnd = Luminance(g_RenderTextureView.SampleGrad(g_Sampler, pointN, pointOffset, pointOffset).rgb); }
		}

		// Search for significant change in luma compared to current highest pair.
		searchNeg = searchNeg || (abs(lumaNegEnd - lumaNeg) >= gradientNeg);
		searchPos = searchPos || (abs(lumaPosEnd - lumaPos) >= gradientPos);


		// Display debug information regarding edges.
		if (FXAA_DEBUG_NEGPOS > 0) {

			bool negGreater = (abs(lumaNegEnd - gradientNeg)) > (abs(lumaPosEnd - gradientPos));

			if (searchNeg && negGreater) {
				return float3(abs(lumaNegEnd - gradientNeg), 0.0, 0.0);
			}
			else if (searchPos && !negGreater) {
				return float3(0.0, 0.0, abs(lumaPosEnd - gradientPos));
			}
			else {
				return float3(abs(lumaNegEnd - gradientNeg), 0.0, abs(lumaPosEnd - gradientPos));
			}

		}

		// Determine if search is over early.
		if (searchNeg && searchPos) { break; }

		// If still searching, increment offset.
		if (!searchNeg) { pointN -= pointOffset; }
		if (!searchPos) { pointP += pointOffset; }
	}

	//-------------------------
	// 6. SUB-PIXEL SHIFT

	// Determine if sub-pixel center falls on positive or negative side.
	float distanceNeg = isHorizontal ? uv.x - pointN.x : uv.y - pointN.y;
	float distancePos = isHorizontal ? pointP.x - uv.x : pointP.y - uv.y;
	bool isCloserToNegative = distanceNeg < distancePos;

	// Assign respective luma.
	float lumaEnd = isCloserToNegative ? lumaNegEnd : lumaPosEnd;

	// Check if pixel is in area that receives no filtering.
	if (((lumaO - lumaNeg) < 0.0) == ((lumaEnd - lumaNeg) < 0.0)) {
		edgeSign = 0.0;
	}

	// Compute sub-pixel offset and filter span.
	float filterSpanLength = (distancePos + distanceNeg);
	float filterDistance = isCloserToNegative ? distanceNeg : distancePos;
	float subpixelOffset = (0.5 + (filterDistance * (-1.0 / filterSpanLength))) * edgeSign;

	if (FXAA_DEBUG_OFFSET > 0)
	{
		if (subpixelOffset < 0.0) {
			// neg-horizontal (red) : neg-vertical (gold)
			float3 red = float3(1.0, 0.0, 0.0);
			float3 gold = float3(1.0, 0.7, 0.0);
			return isHorizontal ? red : gold;
		}

		if (subpixelOffset > 0.0) {
			// pos-horizontal (blue) : pos-vertical (skyblue)
			float3 blue = float3(0.0, 0.0, 1.0);
			float3 skyblue = float3(0.1, 0.3, 1.0);
			return isHorizontal ? blue : skyblue;
		}
	}

	// Resample using the subpixel offset.
	float2 uvOffset = float2(uv.x + (isHorizontal ? 0.0 : subpixelOffset), uv.y + (isHorizontal ? subpixelOffset : 0.0));
	float3 rgbOffset = g_RenderTextureView.SampleLevel(g_Sampler, uvOffset, 0).rgb;

	// return ToFloat3((lumaN + lumaS + lumaE + lumaW + lumaNW + lumaNE + lumaSW + lumaSE) * (1.0/9.0));

	// Display highlight.
	if (FXAA_DEBUG_HIGHLIGHT > 0)
	{
		float3 red = float3(1.0, 0.0, 0.0); // Horizontal.
		float3 green = float3(0.0, 1.0, 0.0); // Vertical.
		return isHorizontal ? red : green;
	}

	// If there is no subpixel aliasing check, we simply output the offset.
	if (FXAA_SUBPIX == 0) { return rgbOffset; }

	// If there is a subpixel aliasing check, we can output the mix.
	return lerp(rgbOffset, rgbL, lowpassBlend);
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