#include "Constants.hlsli"

SunHullPatchOut patch(InputPatch<SunVertexOut, PATCH_SIZE> patch)
{
	SunHullPatchOut output;
	output.Edges[0] = 0.5f * (patch[1].TessFactor + patch[2].TessFactor);
	output.Edges[1] = 0.5f * (patch[2].TessFactor + patch[0].TessFactor);
	output.Edges[2] = 0.5f * (patch[0].TessFactor + patch[1].TessFactor);
	output.Inside = output.Edges[0];
	float3 vFaceNormal = mul((float3x3)gWorldInvTranspose, patch[0].NormalL) +
		mul((float3x3)gWorldInvTranspose, patch[1].NormalL) + 
		mul((float3x3)gWorldInvTranspose, patch[2].NormalL);
	float3 vView = (gEyePosW - mul(gWorld, float4(patch[0].PosL, 1))).xyz;
	if (dot(vView, vFaceNormal) < 0)
	{
		output.Edges[0] = 0;
		output.Edges[1] = 0;
		output.Edges[2] = 0;
		output.Inside = 0;
	}
	return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(PATCH_SIZE)]
[patchconstantfunc("patch")]
SunHullPointOut hull(InputPatch<SunVertexOut, PATCH_SIZE> patch, SunHullIn input)
{
	SunHullPointOut output;
	output.PosL = patch[input.PointID].PosL;
	output.NormalL = patch[input.PointID].NormalL;
	output.Tex = patch[input.PointID].Tex;
	return output;
}
