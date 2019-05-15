#include "Surface.hlsli"

float4 pixel(PixelIn input) : SV_Target
{
	return surface(input);
}

