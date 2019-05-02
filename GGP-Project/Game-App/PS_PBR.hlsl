
#include "Lighting.hlsli"

//Data that changes once per MatMesh combo
cbuffer perCombo : register(b0)
{
	Light Lights[MAX_LIGHTS]; //array of lights
	int LightCount; //amount of lights
	float3 CameraPosition;
	AmbientLight AmbLight;
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
Texture2D RoughnessTexture		: register(t2);
Texture2D MetalTexture			: register(t3);
SamplerState BasicSampler		: register(s0);

// Shadow-related variables
Texture2D ShadowMap						: register(t4);
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

	// Sample the roughness map
	float roughness = RoughnessTexture.Sample(BasicSampler, input.uv).r;

	// Sample the metal map
	float metal = MetalTexture.Sample(BasicSampler, input.uv).r;

	// Sample texture
	float4 surfaceColor = AlbedoTexture.Sample(BasicSampler, input.uv);
	surfaceColor.rgb = pow(surfaceColor.rgb, 2.2);

	// Specular color - Assuming albedo texture is actually holding specular color if metal == 1
	float3 specColor = lerp(F0_NON_METAL.rrr, surfaceColor.rgb, metal);

	//Sample shadowmap
	//Shadows are only on the singular directional light
	float depthFromLight = input.posForShadow.z / input.posForShadow.w;
	float2 shadowUV = input.posForShadow.xy / input.posForShadow.w * 0.5f + 0.5f;
	shadowUV.y = 1.0f - shadowUV.y;
	float shadowAmount = ShadowMap.SampleCmpLevelZero(ShadowSampler, shadowUV, depthFromLight);

	// Total color for this pixel
	float3 totalColor = float3(0,0,0);

	//Add ambient light
	totalColor += AmbLight.Color * AmbLight.Intensity * surfaceColor.rgb;

	// Loop through all lights this frame
	for (int i = 0; i < LightCount; i++)
	{
		// Which kind of light?
		switch (Lights[i].Type)
		{
		case LIGHT_TYPE_DIRECTIONAL:
			float3 dL = DirLightPBR(Lights[i], input.normal, input.worldPos, CameraPosition, roughness, metal, surfaceColor.rgb, specColor);
			dL *= shadowAmount;
			totalColor += dL;
			break;

		case LIGHT_TYPE_POINT:
			float3 pL = PointLightPBR(Lights[i], input.normal, input.worldPos, CameraPosition, roughness, metal, surfaceColor.rgb, specColor);
			//pL *= shadowAmount;
			totalColor += pL;
			break;

		case LIGHT_TYPE_SPOT:
			float3 sL = SpotLightPBR(Lights[i], input.normal, input.worldPos, CameraPosition, roughness, metal, surfaceColor.rgb, specColor);
			//sL *= shadowAmount;
			totalColor += sL;
			break;
		}
	}

	// Adjust the light color by the light amount
	float3 gammaCorrect = pow(totalColor, 1.0 / 2.2);
	return float4(gammaCorrect, 1);
}