#pragma once

#include <stdint.h>
#include <math.h>

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

inline float sRGB_to_Linear(float v)
{
	if (v < 0.04045f)
		return v / 12.92f;
	else
		return powf((v + 0.055f) / 1.055f, 2.4f);
}

inline float Linear_to_sRGB(float v)
{
	if (v < 0.0031308f)
		return 12.92f * v;
	else
		return 1.055f * powf(v, 0.41666f) - 0.055f;
}

uint8_t Linear_to_sRGB_pix(float v);

inline float3 sRGB_to_Linear(float3 v)
{
	return {sRGB_to_Linear(v.x), sRGB_to_Linear(v.y), sRGB_to_Linear(v.z)};
}

inline float3 Linear_to_sRGB(float3 v)
{
	return { Linear_to_sRGB(v.x), Linear_to_sRGB(v.y), Linear_to_sRGB(v.z) };
}

inline pix3 Linear_to_sRGB_pix(float3 v)
{
	return { Linear_to_sRGB_pix(v.x), Linear_to_sRGB_pix(v.y), Linear_to_sRGB_pix(v.z) };
}

inline float3 pix_to_float(pix3 v)
{
	constexpr float sc = 1.0f / 255.0f;
	return {v.r * sc, v.g * sc, v.b * sc};
}

inline pix3 float_to_pix(float3 v)
{
	v.x = saturate(v.x);
	v.y = saturate(v.y);
	v.z = saturate(v.z);
	return {
		(uint8_t)(v.x * 255.0f + 0.5f),
		(uint8_t)(v.y * 255.0f + 0.5f),
		(uint8_t)(v.z * 255.0f + 0.5f)
	};
}
