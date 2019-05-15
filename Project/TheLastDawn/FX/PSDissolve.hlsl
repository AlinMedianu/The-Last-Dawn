#include "Surface.hlsli"

float4 pixel(PixelIn input) : SV_TARGET
{
	half pixelNoise = gNoiseMap.Sample(samAnisotropic, input.Tex).r;
	half remappedTime = (gEyePosW.w + 1) * 0.5;
	clip(pixelNoise - remappedTime);							//edge thickness and colour
	return surface(input, true, false, step(pixelNoise, remappedTime + gThickness) * gColour);
}