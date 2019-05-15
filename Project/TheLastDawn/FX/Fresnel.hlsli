#ifndef FRESNEL
#define FRESNEL

#include "Constants.hlsli"

//use with emission
float fresnel(float3 normalW, float exponent)
{
	//invert the fresnel so the big values are on the outside
	float fresnel = dot(normalize(normalW), float3(0, 0, -1));
	fresnel = saturate(1 - fresnel);
	//adjust the ratio of the fresnel
	return pow(fresnel, exponent);
}

#endif