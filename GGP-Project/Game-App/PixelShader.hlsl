
#include "Lighting.hlsli"

// Struct representing the data for any light

//Data that changes once per MatMesh combo
cbuffer perCombo : register(b0)
{
	Light Lights[MAX_LIGHTS]; //array of lights
	int LightCount; //amount of lights
	float3 CameraPosition;
	AmbientLight AmbLight;
	float Shininess;
	float Roughness;
}

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION; // The world position of this PIXEL
	float4 posForShadow : SHADOW;
};

// Texture-related variables
Texture2D AlbedoTexture			: register(t0);
Texture2D NormalTexture			: register(t1);
SamplerState BasicSampler		: register(s0);

// Shadow-related variables
Texture2D ShadowMap						: register(t2);
SamplerComparisonState ShadowSampler	: register(s1);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	// Fix for poor normals: re-normalizing interpolated normals
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	// Use normal mapping
	float3 normalMap = NormalMapping(NormalTexture, BasicSampler, input.uv, input.normal, input.tangent);
	input.normal = normalMap;

	// Sample texture
	float4 surfaceColor = AlbedoTexture.Sample(BasicSampler, input.uv);
	surfaceColor.rgb = pow(surfaceColor.rgb, 2.2);

	//Sample shadowmap
	//Shadows are only on the singular directional light
	float depthFromLight = input.posForShadow.z / input.posForShadow.w;
	float2 shadowUV = input.posForShadow.xy / input.posForShadow.w * 0.5f + 0.5f;
	shadowUV.y = 1.0f - shadowUV.y;
	float shadowAmount = ShadowMap.SampleCmpLevelZero(ShadowSampler, shadowUV, depthFromLight);

	// Total color for this pixel
	float3 totalColor = float3(0, 0, 0);

	//Add ambient light
	totalColor += AmbLight.Color * AmbLight.Intensity * surfaceColor.rgb;

	// Loop through all lights this frame
	for (int i = 0; i < LightCount; i++)
	{
		// Which kind of light?
		switch (Lights[i].Type)
		{
		case LIGHT_TYPE_DIRECTIONAL:
			float3 dL = DirLight(Lights[i], input.normal, input.worldPos, CameraPosition, Shininess, Roughness, surfaceColor.rgb);
			dL *= shadowAmount;
			totalColor += dL;
			break;

		case LIGHT_TYPE_POINT:
			float3 pL = PointLight(Lights[i], input.normal, input.worldPos, CameraPosition, Shininess, Roughness, surfaceColor.rgb);
			//pL *= shadowAmount;
			totalColor += pL;
			break;

		case LIGHT_TYPE_SPOT:
			float3 sL = SpotLight(Lights[i], input.normal, input.worldPos, CameraPosition, Shininess, Roughness, surfaceColor.rgb);
			//sL *= shadowAmount;
			totalColor += sL;
			break;
		}
	}

	// Adjust the light color by the light amount
	float3 gammaCorrect = pow(totalColor, 1.0 / 2.2);
	return float4(gammaCorrect, 1);
}