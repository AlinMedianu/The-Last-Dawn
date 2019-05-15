#include "Constants.hlsli"

SunVertexOut vertex(SunVertexIn input)
{
	SunVertexOut output;
	output.PosL = input.PosL;
	float d = distance(mul(gWorld, float4(input.PosL, 1.0f)), gEyePosW);
	float tess = saturate((10 - d) / (10 - 40));
	output.TessFactor = lerp(20, 3, tess);
	output.NormalL = input.NormalL;
	output.Tex = input.Tex;
	return output;
}