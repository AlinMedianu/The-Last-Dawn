#include "Constants.hlsli"

float4 pixel(ParticlePixelIn input) : SV_Target
{
	//sum intensities 
	float4 litColor = gDiffuseMap.Sample(samAnisotropic, input.Tex);
	// Common to take alpha from diffuse material.
	litColor.a = gMaterial.Diffuse.a * litColor.a;

	return litColor; 
}