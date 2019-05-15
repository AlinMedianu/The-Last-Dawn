#include "Constants.hlsli"

float4 pixel() : SV_Target
{
	return gMaterial.Diffuse;
}
  
