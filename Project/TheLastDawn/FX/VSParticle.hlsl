#include "Constants.hlsli"

ParticlePixelIn vertex(ParticleVertexIn input)
{
	ParticlePixelIn output;
	
	// Transform to homogeneous clip space.
	output.PosH = mul(gWorldViewProj, float4(input.PosL, 1.0f));

	// Output vertex attributes for interpolation across triangle.
	output.Tex = mul(gTexTransform, float4(input.Tex, 0.0f, 1.0f)).xy;

	return output;
}

