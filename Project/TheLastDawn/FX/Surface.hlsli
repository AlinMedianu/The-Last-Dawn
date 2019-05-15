#ifndef SURFACE
#define SURFACE

#include "Constants.hlsli"

float4 surface(PixelIn input, bool textured = true, bool useEmissionMap = false, float3 emission = float3(0, 0, 0)) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
	input.NormalW = normalize(input.NormalW);

	float3 toEyeW = normalize(gEyePosW.xyz - input.PosW);

	float4 texColor = float4(1, 1, 1, 1);
	//conditional use of the texture without branching
	texColor = textured * gDiffuseMap.Sample(samAnisotropic, input.Tex) +
		!textured * float4(1, 1, 1, 1);

	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	//if one of the emissions is float3(0, 0, 0), but the other one isn't
	//take the other one, without branching
	float4 emissive = max(gMaterial.Emissive, float4(emission, 1));
	//conditional use of the texture without branching
	emissive *= useEmissionMap * gEmissionMap.Sample(samAnisotropic, input.Tex) +
		!useEmissionMap * float4(1, 1, 1, 1);
	// Sum the light contribution from each light source.
	float4 A, D, S;
	[unroll]
	for (int lIdx = 0; lIdx < MAX_LIGHTS; ++lIdx)
	{
		if (gLights[lIdx].Type == LIGHT_DIR)
		{
			ComputeDirectionalLight(gMaterial, gLights[lIdx], input.NormalW, toEyeW, A, D, S);
			ambient += A;
			diffuse += D;
			specular += S;
		}
		else if (gLights[lIdx].Type == LIGHT_POINT)
		{
			ComputePointLight(gMaterial, gLights[lIdx], input.PosW, input.NormalW, toEyeW, A, D, S);
			ambient += A;
			diffuse += D;
			specular += S;
		}
		else if (gLights[lIdx].Type == LIGHT_SPOT)
		{
			ComputeSpotLight(gMaterial, gLights[lIdx], input.PosW, input.NormalW, toEyeW, A, D, S);
			ambient += A;
			diffuse += D;
			specular += S;
		}
	}

	//sum intensities 
	float4 litColor = saturate(texColor * saturate(ambient + diffuse) + specular + emissive);

	// Common to take alpha from diffuse material.
	litColor.a = gMaterial.Diffuse.a * texColor.a;

	return litColor;
}

#endif