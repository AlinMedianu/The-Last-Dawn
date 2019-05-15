#include "Surface.hlsli"
#include "Fresnel.hlsli"
#include "Noise.hlsli"

float4 pixel(SunPixelIn input) : SV_Target
{
	float4 noiseColour = lerp(float4(1, 0, 0.2, 1), float4(1, 1, 0, 1), input.Noise);
	return saturate(gMaterial.Emissive + noiseColour +
		float4(1, 0.5, 0, 1) * fresnel(input.NormalW, distance(input.PosW, gEyePosW.xyz) / 5));
}