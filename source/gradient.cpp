#include "gradient.h"
#include "oklab.h"

void Gradient::Precalc()
{
	for (int i = 0; i < m_KeyCount - 1; ++i)
	{
		m_InvTimeDeltas[i] = 1.0f / (m_Times[i + 1] - m_Times[i]);
	}
}

pix3 Gradient::Evaluate_sRGB(float t) const
{
	// find the keys to interpolate between
	int idx = 0;
	while (idx < m_KeyCount - 1 && t >= m_Times[idx + 1])
		++idx;
	// we are past the last key; just return that
	if (idx >= m_KeyCount - 1)
		return m_Keys[m_KeyCount - 1];

	// interpolate between the keys
	float a = (t - m_Times[idx]) * m_InvTimeDeltas[idx];

	pix3 c = lerp(m_Keys[idx], m_Keys[idx + 1], a);
	return c;
}

pix3 Gradient::Evaluate_Linear(float t) const
{
	// find the keys to interpolate between
	int idx = 0;
	while (idx < m_KeyCount - 1 && t >= m_Times[idx + 1])
		++idx;
	// we are past the last key; just return that
	if (idx >= m_KeyCount - 1)
		return m_Keys[m_KeyCount - 1];

	// interpolate between the keys
	float a = (t - m_Times[idx]) * m_InvTimeDeltas[idx];

	// to-Linear -> lerp -> to-sRGB
	float3 ca = pix_to_float(m_Keys[idx]);
	float3 cb = pix_to_float(m_Keys[idx + 1]);
	ca = sRGB_to_Linear(ca);
	cb = sRGB_to_Linear(cb);
	float3 c = lerp(ca, cb, a);
	c = Linear_to_sRGB(c);
	return float_to_pix(c);
}

pix3 Gradient::Evaluate_OkLab(float t) const
{
	// find the keys to interpolate between
	int idx = 0;
	while (idx < m_KeyCount - 1 && t >= m_Times[idx + 1])
		++idx;
	// we are past the last key; just return that
	if (idx >= m_KeyCount - 1)
		return m_Keys[m_KeyCount - 1];

	// interpolate between the keys
	float a = (t - m_Times[idx]) * m_InvTimeDeltas[idx];

	// to-Linear -> to-Oklab -> lerp -> to-Linear -> to-sRGB
	float3 ca = pix_to_float(m_Keys[idx]);
	float3 cb = pix_to_float(m_Keys[idx + 1]);
	ca = sRGB_to_Linear(ca);
	cb = sRGB_to_Linear(cb);
	ca = Linear_sRGB_to_OkLab_Ref(ca);
	cb = Linear_sRGB_to_OkLab_Ref(cb);
	float3 c = lerp(ca, cb, a);
	c = OkLab_to_Linear_sRGB_Ref(c);
	c = Linear_to_sRGB(c);
	return float_to_pix(c);
}
