#include "Constants.hlsli"

PixelIn vertex(VertexIn input)
{
	PixelIn output;
	
	// Transform to world space space.
	output.PosW = mul(gWorld, float4(input.PosL, 1.0f)).xyz;
	output.NormalW = mul((float3x3)gWorldInvTranspose, input.NormalL);
		
	// Transform to homogeneous clip space.
	output.PosH = mul(gWorldViewProj, float4(input.PosL, 1.0f));

	// Output vertex attributes for interpolation across triangle.
	output.Tex = mul(gTexTransform, float4(input.Tex, 0.0f, 1.0f)).xy;

	return output;
}

