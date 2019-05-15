#ifndef NOISE
#define NOISE

//code taken from: https://www.shadertoy.com/view/XdXGW8
float2 hash(float2 x)
{
	const float2 k = float2(0.3183099, 0.3678794);
	x = x * k + k.yx;
	return -1.0 + 2.0 * frac(16.0 * k * frac(x.x * x.y * (x.x + x.y)));
}

float noise(float2 p)
{
	float2 i = floor(p);
	float2 f = frac(p);

	float2 u = f * f * (3.0 - 2.0 * f);

	return lerp(lerp(dot(hash(i + float2(0, 0)), f - float2(0, 0)),
		dot(hash(i + float2(1, 0)), f - float2(1, 0)), u.x),
		lerp(dot(hash(i + float2(0, 1)), f - float2(0, 1)),
			dot(hash(i + float2(1, 1)), f - float2(1, 1)), u.x), u.y);
}

#endif