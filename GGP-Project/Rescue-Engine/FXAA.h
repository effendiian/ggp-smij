#pragma once

#include <sstream>

// ----------------------------------------------
// enum
//
// Used to toggle values for the flags like a boolean.
// ----------------------------------------------
enum FXAA_BOOLEAN_FLAG {
	OFF = 0,
	ON = 1
};

// ----------------------------------------------
// enum
//
// Used to set the FXAA preset value.
// ----------------------------------------------
enum FXAA_PRESET_FLAG {

	// --------------------------------------------------------------
	// DEFAULT_FXAA_PRESET, FXAA_PRESET_0
	//
	// Minimum quality preset is used.
	//		#define FXAA_EDGE_THRESHOLD      (1.0/4.0)
	//		#define FXAA_EDGE_THRESHOLD_MIN  (1.0/12.0)
	//		#define FXAA_SEARCH_STEPS        2
	//		#define FXAA_SEARCH_ACCELERATION 4
	//		#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
	//		#define FXAA_SUBPIX              1
	//		#define FXAA_SUBPIX_FASTER       1
	//		#define FXAA_SUBPIX_CAP          (2.0/3.0)
	//		#define FXAA_SUBPIX_TRIM         (1.0/4.0)
	// --------------------------------------------------------------
	DEFAULT_FXAA_PRESET = 0,
	FXAA_PRESET_0 = 0,

	// --------------------------------------------------------------
	// FXAA_PRESET_1
	//
	// Set FXAA to preset 1.
	//		#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
	//		#define FXAA_EDGE_THRESHOLD_MIN  (1.0/16.0)
	//		#define FXAA_SEARCH_STEPS        4
	//		#define FXAA_SEARCH_ACCELERATION 3
	//		#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
	//		#define FXAA_SUBPIX              1
	//		#define FXAA_SUBPIX_FASTER       0
	//		#define FXAA_SUBPIX_CAP          (3.0/4.0)
	//		#define FXAA_SUBPIX_TRIM         (1.0/4.0)
	// --------------------------------------------------------------
	FXAA_PRESET_1 = 1,

	// --------------------------------------------------------------
	// FXAA_PRESET_2
	//
	// Set FXAA to preset 2.
	//		#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
	//		#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
	//		#define FXAA_SEARCH_STEPS        8
	//		#define FXAA_SEARCH_ACCELERATION 2
	//		#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
	//		#define FXAA_SUBPIX              1
	//		#define FXAA_SUBPIX_FASTER       0
	//		#define FXAA_SUBPIX_CAP          (3.0/4.0)
	//		#define FXAA_SUBPIX_TRIM         (1.0/4.0)
	// --------------------------------------------------------------
	FXAA_PRESET_2 = 2,

	// --------------------------------------------------------------
	// FXAA_PRESET_3
	//
	// Set FXAA to preset 3.
	//		#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
	//		#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
	//		#define FXAA_SEARCH_STEPS        16
	//		#define FXAA_SEARCH_ACCELERATION 1
	//		#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
	//		#define FXAA_SUBPIX              1
	//		#define FXAA_SUBPIX_FASTER       0
	//		#define FXAA_SUBPIX_CAP          (3.0/4.0)
	//		#define FXAA_SUBPIX_TRIM         (1.0/4.0)
	// --------------------------------------------------------------
	FXAA_PRESET_3 = 3,

	// --------------------------------------------------------------
	// FXAA_PRESET_4
	//
	// Set FXAA to preset 4.
	//		#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
	//		#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
	//		#define FXAA_SEARCH_STEPS        24
	//		#define FXAA_SEARCH_ACCELERATION 1
	//		#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
	//		#define FXAA_SUBPIX              1
	//		#define FXAA_SUBPIX_FASTER       0
	//		#define FXAA_SUBPIX_CAP          (3.0/4.0)
	//		#define FXAA_SUBPIX_TRIM         (1.0/4.0)
	// --------------------------------------------------------------
	FXAA_PRESET_4 = 4,

	// --------------------------------------------------------------
	// FXAA_PRESET_5
	//
	// Set FXAA to preset 5.
	//		#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
	//		#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
	//		#define FXAA_SEARCH_STEPS        32
	//		#define FXAA_SEARCH_ACCELERATION 1
	//		#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
	//		#define FXAA_SUBPIX              1
	//		#define FXAA_SUBPIX_FASTER       0
	//		#define FXAA_SUBPIX_CAP          (3.0/4.0)
	//		#define FXAA_SUBPIX_TRIM         (1.0/4.0)
	// --------------------------------------------------------------
	FXAA_PRESET_5 = 5,

	// --------------------------------------------------------------
	// FXAA_CUSTOM
	//
	// Individual properties are sampled without evaluating the preset.
	// --------------------------------------------------------------
	FXAA_PRESET_CUSTOM = 6
};

// ----------------------------------------------
// enum
//
// Used to set luminance calculation settings.
// ----------------------------------------------
enum FXAA_LUMINANCE_FLAG {

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
	// Luma used is equivalent to the raw red. (Rare).
	// --------------------------------------------------------------
	RED = 4,

	// --------------------------------------------------------------
	// RED_GAMMA - sqrt(color.r)
	//
	// Luma used is approximate to the gamma-correct red. (Rare).
	// --------------------------------------------------------------
	RED_GAMMA = 5,

	// --------------------------------------------------------------
	// GREEN - color.g
	//
	// Luma used is equivalent to the raw green. (Common).
	// --------------------------------------------------------------
	GREEN = 6,

	// --------------------------------------------------------------
	// GREEN_GAMMA - sqrt(color.g)
	//
	// Luma used is approximate to the gamma-correct green. (Common).
	// --------------------------------------------------------------
	GREEN_GAMMA = 7,

	// --------------------------------------------------------------
	// BLUE - color.b
	//
	// Luma used is equivalent to the raw blue. (Rare).
	// --------------------------------------------------------------
	BLUE = 8,

	// --------------------------------------------------------------
	// BLUE_GAMMA - sqrt(color.b)
	//
	// Luma used is approximate to the gamma-correct blue. (Rare).
	// --------------------------------------------------------------
	BLUE_GAMMA = 9
};

// ----------------------------------------------
// struct
//
// Contains preference information for FXAA post-processing.
// Default values based on FXAA_PRESET == 0.
// ----------------------------------------------
struct FXAA_DESC
{
public:

	// -------------
	// Constructor.

	FXAA_DESC() {};
	~FXAA_DESC() {};

	// -------------
	// Constants.

	// DEFAULT SETTINGS == FXAA_PRESET as 0.
	const unsigned int DEFAULT_PRESET = FXAA_PRESET_FLAG::DEFAULT_FXAA_PRESET;
	const float DEFAULT_EDGE_THRESHOLD = (1.0f / 4.0f);
	const float DEFAULT_EDGE_THRESHOLD_MIN = (1.0f / 12.0f);
	const unsigned int DEFAULT_SEARCH_STEPS = 2;
	const unsigned int DEFAULT_SEARCH_ACCELERATION = 4;
	const float DEFAULT_SEARCH_THRESHOLD = (1.0f / 4.0f);
	const unsigned int DEFAULT_SUBPIX = FXAA_BOOLEAN_FLAG::ON;
	const unsigned int DEFAULT_SUBPIX_FASTER = FXAA_BOOLEAN_FLAG::ON;
	const float DEFAULT_SUBPIX_CAP = (2.0f / 3.0f);
	const float DEFAULT_SUBPIX_TRIM = (1.0f / 4.0f);
	const unsigned int DEFAULT_LUMINANCE_METHOD = FXAA_LUMINANCE_FLAG::REVISED;

	// -------------
	// Properties.

	// --------------------------------------
	// FXAA
	//
	// Determine if FXAA should be performed.
	// 0 - off
	// 1 - on
	// --------------------------------------
	unsigned int FXAA = FXAA_BOOLEAN_FLAG::ON;

	// --------------------------------------
	// FXAA_PRESET
	//
	// Assign preset to generate values.
	// 0 - Use standard FXAA Preset 0.
	// 1 - Use standard FXAA Preset 1.
	// 2 - Use standard FXAA Preset 2.
	// 3 - Use standard FXAA Preset 3.
	// 4 - Use standard FXAA Preset 4.
	// 5 - Use standard FXAA Preset 5.
	// 6 - Ignore the FXAA standard presets and use custom values.
	// --------------------------------------
	unsigned int FXAA_PRESET = DEFAULT_PRESET;

	// --------------------------------------
	// FXAA_EDGE_TRESHOLD
	//
	// Threshold used to determine high contrast change.
	// Preset 0 - (1.0/4.0)
	// Preset 1 - (1.0/8.0)
	// Preset 2 - (1.0/8.0)
	// Preset 3 - (1.0/8.0)
	// Preset 4 - (1.0/8.0)
	// Preset 5 - (1.0/8.0)
	// --------------------------------------
	float EDGE_THRESHOLD = DEFAULT_EDGE_THRESHOLD;

	// --------------------------------------
	// FXAA_EDGE_TRESHOLD_MIN
	//
	// Minimum threshold used to determine high contrast change.
	// Preset 0 - (1.0/12.0)
	// Preset 1 - (1.0/16.0)
	// Preset 2 - (1.0/24.0)
	// Preset 3 - (1.0/24.0)
	// Preset 4 - (1.0/24.0)
	// Preset 5 - (1.0/24.0)
	// --------------------------------------
	float EDGE_THRESHOLD_MIN = DEFAULT_EDGE_THRESHOLD_MIN;

	// --------------------------------------
	// FXAA_SEARCH_STEPS
	//
	// Steps to take during end-of-edge detection algorithm.
	// Preset 0 - 2
	// Preset 1 - 4
	// Preset 2 - 8
	// Preset 3 - 16
	// Preset 4 - 24
	// Preset 5 - 32
	// --------------------------------------
	unsigned int SEARCH_STEPS = DEFAULT_SEARCH_STEPS;

	// --------------------------------------
	// FXAA_SEARCH_ACCELERATION
	//
	// Determine if end-of-edge search should be accelerated.
	// 0 - off
	// 1 - on
	// --------------------------------------
	unsigned int SEARCH_ACCELERATION = DEFAULT_SEARCH_ACCELERATION;

	// --------------------------------------
	// FXAA_SEARCH_THRESHOLD
	//
	// Threshold used to determine high contrast change during end-of-edge search.
	// Preset 0 - (1.0/4.0)
	// Preset 1 - (1.0/4.0)
	// Preset 2 - (1.0/4.0)
	// Preset 3 - (1.0/4.0)
	// Preset 4 - (1.0/4.0)
	// Preset 5 - (1.0/4.0)
	// --------------------------------------
	float SEARCH_THRESHOLD = DEFAULT_SEARCH_THRESHOLD;

	// --------------------------------------
	// FXAA_SUBPIX
	//
	// Determine if subpixel aliasing should take place.
	// 0 - off
	// 1 - on
	// --------------------------------------
	unsigned int SUBPIX = DEFAULT_SUBPIX;

	// --------------------------------------
	// FXAA_SUBPIX_FASTER
	//
	// Allow subpixel aliasing to resolve faster for performance reasons.
	// 0 - off
	// 1 - on
	// --------------------------------------
	unsigned int SUBPIX_FASTER = DEFAULT_SUBPIX_FASTER;

	// --------------------------------------
	// FXAA_SUBPIX_CAP
	//
	// Maximum cap to lowpass blending during subpixel aliasing.
	// Preset 0 - (2.0/3.0)
	// Preset 1 - (3.0/4.0)
	// Preset 2 - (3.0/4.0)
	// Preset 3 - (3.0/4.0)
	// Preset 4 - (3.0/4.0)
	// Preset 5 - (3.0/4.0)
	// --------------------------------------
	float SUBPIX_CAP = DEFAULT_SUBPIX_CAP;

	// --------------------------------------
	// FXAA_SUBPIX_TRIM
	//
	// Amount to trim from contrast during subpixel aliasing.
	// Preset 0 - (1.0/4.0)
	// Preset 1 - (1.0/4.0)
	// Preset 2 - (1.0/4.0)
	// Preset 3 - (1.0/4.0)
	// Preset 4 - (1.0/4.0)
	// Preset 5 - (1.0/4.0)
	// --------------------------------------
	float SUBPIX_TRIM = DEFAULT_SUBPIX_TRIM;

	// --------------------------------------
	// FXAA_LUMINANCE_METHOD
	//
	// Luminance calculation method to use.
	// 0 - RGBL ("off")
	// 1 - ORIGINAL
	// 2 - REVISED
	// 3 - LINEAR
	// 4 - RED
	// 5 - RED_GAMMA
	// 6 - GREEN
	// 7 - GREEN_GAMMA
	// 8 - BLUE
	// 9 - BLUE_GAMMA
	// --------------------------------------
	unsigned int LUMINANCE_METHOD = DEFAULT_LUMINANCE_METHOD;

	// --------------------------------------
	// FXAA_DEBUG_DISCARD
	//
	// Discard pixels on early exit.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int DEBUG_DISCARD = FXAA_BOOLEAN_FLAG::OFF;

	// --------------------------------------
	// FXAA_DEBUG_PASSTHROUGH
	//
	// Display local contrast value of pixels affected by subpixel aliasing.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int DEBUG_PASSTHROUGH = FXAA_BOOLEAN_FLAG::OFF;

	// --------------------------------------
	// FXAA_DEBUG_HORZVERT
	//
	// Highlight pixels discovered by the horizontal/vertical edge test.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int DEBUG_HORZVERT = FXAA_BOOLEAN_FLAG::OFF;

	// --------------------------------------
	// FXAA_DEBUG_PAIR
	//
	// Highlight highest orthogonal contrast pair for a particular point.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int DEBUG_PAIR = FXAA_BOOLEAN_FLAG::OFF;

	// --------------------------------------
	// FXAA_DEBUG_NEGPOS
	//
	// Highlight the negative/positive end-of-edge search results.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int DEBUG_NEGPOS = FXAA_BOOLEAN_FLAG::OFF;

	// --------------------------------------
	// FXAA_DEBUG_OFFSET
	//
	// Highlight pixels affected by subpixel offset calculations.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int DEBUG_OFFSET = FXAA_BOOLEAN_FLAG::OFF;

	// --------------------------------------
	// FXAA_DEBUG_HIGHLIGHT
	//
	// Highlight pixels affected by the FXAA process.
	// 0 - off.
	// 1 - on.
	// --------------------------------------
	unsigned int DEBUG_HIGHLIGHT = FXAA_BOOLEAN_FLAG::OFF;

	// --------------------------------------
	// FXAA_DEBUG_GRAYSCALE
	//
	// Percentage [0..1] of grayscale amount to apply to final output.
	// --------------------------------------
	float DEBUG_GRAYSCALE = 0.0;

	// --------------------------------------
	// FXAA_DEBUG_GRAYSCALE_CHANNEL
	//
	// Channel to base grayscale off of.
	// 0 - r
	// 1 - g (Used by default).
	// 2 - b
	// 3 - a
	// --------------------------------------
	unsigned int DEBUG_GRAYSCALE_CHANNEL = 1;

	// -------------
	// Methods.

	// Overwrites values based on input preset. (Custom preset does nothing).
	void LoadPreset(unsigned int _presetID);

	// Initializes values to the default.
	void Init();

private:

	// -------------
	// Constants.

	// PRESET 0
	const float PRESET0_EDGE_THRESHOLD = (1.0f / 4.0f);
	const float PRESET0_EDGE_THRESHOLD_MINIMUM = (1.0f / 12.0f);
	const unsigned int PRESET0_SEARCH_STEPS = 2;
	const unsigned int PRESET0_SEARCH_ACCELERATION = 4;
	const float PRESET0_SEARCH_THRESHOLD = (1.0f / 4.0f);
	const unsigned int PRESET0_SUBPIX = FXAA_BOOLEAN_FLAG::ON;
	const unsigned int PRESET0_SUBPIX_FASTER = FXAA_BOOLEAN_FLAG::ON;
	const float PRESET0_SUBPIX_CAP = (2.0f / 3.0f);
	const float PRESET0_SUBPIX_TRIM = (1.0f / 4.0f);

	// PRESET 1
	const float PRESET1_EDGE_THRESHOLD = (1.0f / 8.0f);
	const float PRESET1_EDGE_THRESHOLD_MINIMUM = (1.0f / 16.0f);
	const unsigned int PRESET1_SEARCH_STEPS = 4;
	const unsigned int PRESET1_SEARCH_ACCELERATION = 3;
	const float PRESET1_SEARCH_THRESHOLD = (1.0f / 4.0f);
	const unsigned int PRESET1_SUBPIX = FXAA_BOOLEAN_FLAG::ON;
	const unsigned int PRESET1_SUBPIX_FASTER = FXAA_BOOLEAN_FLAG::OFF;
	const float PRESET1_SUBPIX_CAP = (3.0f / 4.0f);
	const float PRESET1_SUBPIX_TRIM = (1.0f / 4.0f);

	// PRESET 2
	const float PRESET2_EDGE_THRESHOLD = (1.0f / 8.0f);
	const float PRESET2_EDGE_THRESHOLD_MINIMUM = (1.0f / 24.0f);
	const unsigned int PRESET2_SEARCH_STEPS = 8;
	const unsigned int PRESET2_SEARCH_ACCELERATION = 2;
	const float PRESET2_SEARCH_THRESHOLD = (1.0f / 4.0f);
	const unsigned int PRESET2_SUBPIX = FXAA_BOOLEAN_FLAG::ON;
	const unsigned int PRESET2_SUBPIX_FASTER = FXAA_BOOLEAN_FLAG::OFF;
	const float PRESET2_SUBPIX_CAP = (3.0f / 4.0f);
	const float PRESET2_SUBPIX_TRIM = (1.0f / 4.0f);

	// PRESET 3
	const float PRESET3_EDGE_THRESHOLD = (1.0f / 8.0f);
	const float PRESET3_EDGE_THRESHOLD_MINIMUM = (1.0f / 24.0f);
	const unsigned int PRESET3_SEARCH_STEPS = 16;
	const unsigned int PRESET3_SEARCH_ACCELERATION = 1;
	const float PRESET3_SEARCH_THRESHOLD = (1.0 / 4.0);
	const unsigned int PRESET3_SUBPIX = FXAA_BOOLEAN_FLAG::ON;
	const unsigned int PRESET3_SUBPIX_FASTER = FXAA_BOOLEAN_FLAG::OFF;
	const float PRESET3_SUBPIX_CAP = (3.0f / 4.0f);
	const float PRESET3_SUBPIX_TRIM = (1.0f / 4.0f);

	// PRESET 4
	const float PRESET4_EDGE_THRESHOLD = (1.0f / 8.0f);
	const float PRESET4_EDGE_THRESHOLD_MINIMUM = (1.0f / 24.0f);
	const unsigned int PRESET4_SEARCH_STEPS = 24;
	const unsigned int PRESET4_SEARCH_ACCELERATION = 1;
	const float PRESET4_SEARCH_THRESHOLD = (1.0f / 4.0f);
	const unsigned int PRESET4_SUBPIX = FXAA_BOOLEAN_FLAG::ON;
	const unsigned int PRESET4_SUBPIX_FASTER = FXAA_BOOLEAN_FLAG::OFF;
	const float PRESET4_SUBPIX_CAP = (3.0f / 4.0f);
	const float PRESET4_SUBPIX_TRIM = (1.0f / 4.0f);

	// PRESET 5
	const float PRESET5_EDGE_THRESHOLD = (1.0f / 8.0f);
	const float PRESET5_EDGE_THRESHOLD_MINIMUM = (1.0f / 24.0f);
	const unsigned int PRESET5_SEARCH_STEPS = 32;
	const unsigned int PRESET5_SEARCH_ACCELERATION = 1;
	const float PRESET5_SEARCH_THRESHOLD = (1.0f / 4.0f);
	const unsigned int PRESET5_SUBPIX = FXAA_BOOLEAN_FLAG::ON;
	const unsigned int PRESET5_SUBPIX_FASTER = FXAA_BOOLEAN_FLAG::OFF;
	const float PRESET5_SUBPIX_CAP = (3.0f / 4.0f);
	const float PRESET5_SUBPIX_TRIM = (1.0f / 4.0f);

	// -------------
	// Methods.

	// Loads preset values.
	void LoadPreset0();
	void LoadPreset1();
	void LoadPreset2();
	void LoadPreset3();
	void LoadPreset4();
	void LoadPreset5();

};