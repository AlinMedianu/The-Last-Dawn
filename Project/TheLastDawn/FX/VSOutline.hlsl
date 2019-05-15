#include "Constants.hlsli"

OutlinePixelIn vertex(OutlineVertexIn input)
{
	OutlinePixelIn output;
	input.PosL.xyz += gThickness * normalize(input.PosL.xyz);
	output.PosH = mul(gWorldViewProj, float4(input.PosL, 1));
	return output;
}