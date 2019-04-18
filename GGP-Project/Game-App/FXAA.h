#pragma once

// ----------------------------------------------
// enum
//
// Used to toggle values for the flags like a boolean.
// ----------------------------------------------
enum FXAABooleanFlag {
	OFF = 0,
	ON = 1
};

// ----------------------------------------------
// enum
//
// Used to set luminance calculation settings.
// ----------------------------------------------
enum FXAALuminance {

	// --------------------------------------------------------------
	// RGBL - Luma contained in alpha channel.
	//
	// Luma is input with color as the alpha value. No calculations.
	// --------------------------------------------------------------
	RGBL = 0,

	// --------------------------------------------------------------
	// ORIGINAL - dot(color.rgb, vec3(0.299, 0.587, 0.114)).
	//
	// Luma calculated using function from the original FXAA source specification.
	// --------------------------------------------------------------
	ORIGINAL = 1,

	// --------------------------------------------------------------
	// REVISED - color.g * (0.587/0.299) + color.r
	//
	// Luma calculated using function from the revised FXAA 3.11 source specification.
	// --------------------------------------------------------------
	REVISED = 2,

	// --------------------------------------------------------------
	// LINEAR - dot(color.rgb, vec3(0.2126729, 0.7151522, 0.0721750));
	//
	// Luma calculated using standard weights for relative luma from https://en.wikipedia.org/wiki/Luma_(video).
	// --------------------------------------------------------------
	LINEAR = 3,

	// --------------------------------------------------------------
	// RED - color.r
	//
	// Luma is equivalent to the raw red. (Rare).
	// --------------------------------------------------------------
	RED = 4,

	// --------------------------------------------------------------
	// RED_GAMMA - sqrt(color.r)
	//
	// Luma is approximate to the gamma-correct red. (Rare).
	// --------------------------------------------------------------
	RED_GAMMA = 5,

	// --------------------------------------------------------------
	// GREEN - color.g
	//
	// Luma is equivalent to the raw green. (Common).
	// --------------------------------------------------------------
	GREEN = 6,

	// --------------------------------------------------------------
	// GREEN_GAMMA - sqrt(color.g)
	//
	// Luma is approximate to the gamma-correct green. (Common).
	// --------------------------------------------------------------
	GREEN_GAMMA = 7,

	// --------------------------------------------------------------
	// BLUE - color.b
	//
	// Luma is equivalent to the raw blue. (Rare).
	// --------------------------------------------------------------
	BLUE = 8,

	// --------------------------------------------------------------
	// BLUE_GAMMA - sqrt(color.b)
	//
	// Luma is approximate to the gamma-correct blue. (Rare).
	// --------------------------------------------------------------
	BLUE_GAMMA = 9
};

// ----------------------------------------------
// struct
//
// Contains preference information for FXAA post-processing.
// Default values based on FXAA_PRESET == 0.
// ----------------------------------------------
struct FXAADesc
{
public:

	// -------------
	// Constructor.

	FXAADesc() {};
	~FXAADesc() {};

	// -------------
	// Properties.



	// -------------
	// Methods.

private:

	float _edgeThreshold = (1.0 / 4.0);
	float _edgeThresholdMinimum = (1.0 / 12.0);
	unsigned int _searchSteps = FXAABooleanFlag::ON * 2;
	unsigned int _searchAcceleration = FXAABooleanFlag::ON * 4;
	float _searchThreshold = (1.0 / 4.0);
	unsigned int _subpix = FXAABooleanFlag::ON;
	unsigned int _subpixFaster = FXAABooleanFlag::ON;
	float _subpixCap = (2.0 / 3.0);
	float _subpixTrim = (1.0 / 4.0);

};

// ----------------------------------------------
// struct
//
// Contains preference information for FXAA post-processing along with special debug information.
// ----------------------------------------------

// Debug description settings.
struct FXAADebugDesc : FXAADesc
{
public:

	// -------------
	// Constructor.

	FXAADebugDesc() : FXAADesc() {};
	~FXAADebugDesc() {};

	// -------------
	// Properties.

	// --------------------------------------
	// FXAA_DEBUG_DISCARD
	//
	// Discard pixels on early exit.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int DISCARD = FXAABooleanFlag::OFF;

	// --------------------------------------
	// FXAA_DEBUG_PASSTHROUGH
	//
	// Display local contrast value of pixels affected by subpixel aliasing.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int PASSTHROUGH = FXAABooleanFlag::OFF;

	// --------------------------------------
	// FXAA_DEBUG_HORZVERT
	//
	// Highlight pixels discovered by the horizontal/vertical edge test.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int HORZVERT = FXAABooleanFlag::OFF;

	// --------------------------------------
	// FXAA_DEBUG_PAIR
	//
	// Highlight highest orthogonal contrast pair for a particular point.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int PAIR = FXAABooleanFlag::OFF;

	// --------------------------------------
	// FXAA_DEBUG_NEGPOS
	//
	// Highlight the negative/positive end-of-edge search results.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int NEGPOS = FXAABooleanFlag::OFF;

	// --------------------------------------
	// FXAA_DEBUG_OFFSET
	//
	// Highlight pixels affected by subpixel offset calculations.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int OFFSET = FXAABooleanFlag::OFF;

	// --------------------------------------
	// FXAA_DEBUG_HIGHLIGHT
	//
	// Highlight pixels affected by the FXAA process.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int HIGHLIGHT = FXAABooleanFlag::OFF;

	// -------------
	// Methods.

	// Set all debug flags simultaneously. (Useful for turning off en masse).
	void SetDebugFlags(FXAABooleanFlag flag);

};