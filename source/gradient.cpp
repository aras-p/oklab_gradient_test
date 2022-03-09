#include "gradient.h"
#include "colorspace.h"

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
		m_KeysOkLab[i] = Linear_sRGB_to_OkLab(c);
		m_KeysLMS[i] = Linear_sRGB_to_LMS(c);
	}
}

template<typename lerp_func>
pix3 Gradient::EvaluateImpl(float t, lerp_func lrp, const float3* precalcKeys) const
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
	return lrp(m_Keys, precalcKeys, idx, a);
}

static pix3 Lerp_sRGB(const pix3* keys, const float3* precalcKeys, int idx, float a)
{
	return lerp(keys[idx], keys[idx + 1], a);
}

static pix3 Lerp_Linear(const pix3* keys, const float3* precalcKeys, int idx, float a)
{
	float3 c = lerp(precalcKeys[idx], precalcKeys[idx + 1], a);
	return Linear_to_sRGB_pix(c);
}

static pix3 Lerp_OkLab(const pix3* keys, const float3* precalcKeys, int idx, float a)
{
	float3 c = lerp(precalcKeys[idx], precalcKeys[idx + 1], a);
	c = OkLab_to_Linear_sRGB(c);
	return Linear_to_sRGB_pix(c);
}

static pix3 Lerp_LMS(const pix3* keys, const float3* precalcKeys, int idx, float a)
{
	float3 c = lerp(precalcKeys[idx], precalcKeys[idx + 1], a);
	c = LMS_to_Linear_sRGB(c);
	return Linear_to_sRGB_pix(c);
}

pix3 Gradient::Evaluate_sRGB(float t) const { return EvaluateImpl(t, Lerp_sRGB, nullptr); }
pix3 Gradient::Evaluate_Linear(float t) const { return EvaluateImpl(t, Lerp_Linear, m_KeysLinear); }
pix3 Gradient::Evaluate_OkLab(float t) const { return EvaluateImpl(t, Lerp_OkLab, m_KeysOkLab); }
pix3 Gradient::Evaluate_LMS(float t) const { return EvaluateImpl(t, Lerp_LMS, m_KeysLMS); }
