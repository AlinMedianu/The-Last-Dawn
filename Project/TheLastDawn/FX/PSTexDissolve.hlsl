#include "Surface.hlsl"

float4 pixel(PixelIn input) : SV_TARGET
{
	half pixelNoise = gNoiseMap.Sample(samAnisotropic, input.Tex).r;
	half remappedTime = (gEyePosW.w + 1) * 0.5;
	clip(pixelNoise - remappedTime);								//edge thickness and colour
	return saturate(surface(input) + float4(step(pixelNoise, remappedTime + gThickness) * gColour, 1));
}