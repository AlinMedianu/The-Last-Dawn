#include "Constants.hlsl"

//data in and out
struct VertexIn
{
  float3 PosL		: POSITION;
  float3 NormalL	: NORMAL;
  float2 Tex		: TEXCOORD;
};

struct VertexOut
{
  float4 PosH		: SV_POSITION;
  float3 PosW		: POSITION;
  float3 NormalW	: NORMAL;
  float2 Tex		: TEXCOORD;
};