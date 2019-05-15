#ifndef Constants
#define Constants

#include "LightHelper.hlsl"

#define MAX_LIGHTS 8
#define LIGHT_OFF 0
#define LIGHT_DIR 1
#define LIGHT_POINT 2
#define LIGHT_SPOT 3


cbuffer cbPerFrame : register(b0)  
{
	Light gLights[MAX_LIGHTS];
	float4 gEyePosW;		//w is a function of time for the dissolving shaders and empty for every other shader
};

cbuffer cbPerObject : register(b1)
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
};

cbuffer cbPerMesh : register(b2)
{
	float4x4 gTexTransform;
	Material gMaterial;
}

cbuffer cbPerCustomShadedObject : register(b3)
{
	float3 gColour;
	float gThickness;
}

Texture2D gDiffuseMap : register(t0);
Texture2D gNoiseMap : register(t1);
Texture2D gEmissionMap	: register(t2);

SamplerState samAnisotropic : register(s0);

//Defaults
struct VertexIn
{
  float3 PosL				: POSITION;
  float3 NormalL			: NORMAL;
  float2 Tex				: TEXCOORD;
};

struct PixelIn
{
  float4 PosH				: SV_POSITION;
  float3 PosW				: POSITION;
  float3 NormalW			: NORMAL;
  float2 Tex				: TEXCOORD;
};

//Particles

struct ParticleVertexIn
{
  float3 PosL				: POSITION;
  float2 Tex				: TEXCOORD;
};

struct ParticlePixelIn
{
  float4 PosH				: SV_POSITION;
  float2 Tex				: TEXCOORD;
};

//Outlines

struct OutlineVertexIn
{
	float3 PosL				: POSITION;
};

struct OutlinePixelIn
{
	float4 PosH				: SV_POSITION;
};

//Sun
#define PatchSize 3

struct SunVertexIn
{

};

struct SunVertexOut
{
	float3 PosW				: POSITION;
	float3 NormalW			: NORMAL;
	float TessFactor		: TESSS;
};

struct SunHullPatchIn
{
	uint PatchID			: SV_PrimitiveID;
};

struct SunHullPointIn
{
	uint PointID			: SV_OutputControlPointID;
	uint PatchID			: SV_PrimitiveID;
};

struct SunHullPatchOut
{
	float Edges[PatchSize]	: SV_TessFactor;
	float Inside			: SV_InsideTessFactor;
};

struct SunHullPointOut
{

};

struct SunDomainIn
{
	SunHullPatchOut PatchData;
	float3 Domain			: SV_DomainLocation;
};

struct SunPixelIn
{

};
#endif