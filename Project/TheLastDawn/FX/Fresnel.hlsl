#ifndef Fresnel
#define Fresnel

#include "Constants.hlsli"

//use with emission
float4 fresnel(PixelIn input, float4 colour, float exponent)
{
	float viewDirection = normalize(gEyePosW.xyz - input.PosW);
	//invert the fresnel so the big values are on the outside
	float fresnel = dot(input.NormalW, viewDirection);
	fresnel = saturate(1 - fresnel);
	//adjust the ratio of the fresnel
	fresnel = pow(fresnel, exponent);
	return colour * fresnel;
}

#endif