#pragma once

#include <stdint.h>

struct pix3
{	
	uint8_t r, g, b;
};

struct float3
{
	float x, y, z;
};

inline float saturate(float v)
{
	if (v < 0) v = 0;
	if (v > 1) v = 1;
	return v;
}

inline float lerp(float a, float b, float t)
{
	return (1 - t) * a + t * b;
}

inline float3 lerp(float3 a, float3 b, float t)
{
	return {
		lerp(a.x, b.x, t),
		lerp(a.y, b.y, t),
		lerp(a.z, b.z, t)
	};
}

inline pix3 lerp(pix3 a, pix3 b, float t)
{
	int scale = (int)(t * 255);
	pix3 c;
	c.r = a.r + ((scale * (b.r - a.r) + 128) >> 8);
	c.g = a.g + ((scale * (b.g - a.g) + 128) >> 8);
	c.b = a.b + ((scale * (b.b - a.b) + 128) >> 8);
	return c;
}
