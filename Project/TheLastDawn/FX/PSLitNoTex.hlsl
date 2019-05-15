#include "Surface.hlsl"

float4 pixel(PixelIn input) : SV_Target
{
	return surface(input, false);
}
