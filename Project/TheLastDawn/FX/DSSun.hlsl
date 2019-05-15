#include "Constants.hlsli"
#include "Noise.hlsli"

[domain("tri")]
SunPixelIn domain(const OutputPatch<SunVertexOut, PATCH_SIZE> patch, SunDomainIn input)
{
	SunPixelIn output;
	float4 posL = float4(patch[0].PosL * input.Domain.x +
		patch[1].PosL * input.Domain.y + patch[2].PosL * input.Domain.z, 1);
	output.PosH = mul(gWorldViewProj, posL);
	output.PosW = mul(gWorld, posL).xyz;
	output.NormalW = mul((float3x3)gWorldInvTranspose, patch[0].NormalL * input.Domain.x +
		patch[1].NormalL * input.Domain.y + patch[2].NormalL * input.Domain.z);
	float2 tex = mul(gTexTransform, float4(patch[0].Tex * input.Domain.x +
		patch[1].Tex * input.Domain.y +
		patch[2].Tex * input.Domain.z, 0, 1)).xy;
	output.Noise = noise(16 * tex);
	output.PosH.xyz += output.NormalW * output.Noise * 100;
	return output;
}
