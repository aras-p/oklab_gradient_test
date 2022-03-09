#include "gradient.h"
#include "oklab.h"

void Gradient::Precalc()
{
	for (int i = 0; i < m_KeyCount - 1; ++i)
	{
		m_InvTimeDeltas[i] = 1.0f / (m_Times[i + 1] - m_Times[i]);
	}
	for (int i = 0; i < m_KeyCount; ++i)
	{
		float3 c = pix_to_float(m_Keys[i]);
		c = sRGB_to_Linear(c);
		m_KeysLinear[i] = c;
		c = Linear_sRGB_to_OkLab_Ref(c);
		m_KeysOkLab[i] = c;
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

	// [precalc to-Linear] -> lerp -> to-sRGB
	float3 c = lerp(m_KeysLinear[idx], m_KeysLinear[idx + 1], a);
	return Linear_to_sRGB_pix(c);
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

	// [precalc to-Linear -> to-Oklab] -> lerp -> to-Linear -> to-sRGB
	float3 c = lerp(m_KeysOkLab[idx], m_KeysOkLab[idx + 1], a);
	c = OkLab_to_Linear_sRGB_Ref(c);
	return Linear_to_sRGB_pix(c);
}
