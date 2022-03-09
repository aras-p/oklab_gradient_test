#include "gradient.h"

void Gradient::Precalc()
{
	for (int i = 0; i < m_KeyCount - 1; ++i)
	{
		m_InvTimeDeltas[i] = 1.0f / (m_Times[i + 1] - m_Times[i]);
	}
}

pix3 Gradient::Evaluate(float t) const
{
	// find the keys to interpolate between
	int idx = 0;
	while (idx < m_KeyCount - 1 && t >= m_Times[idx + 1])
		++idx;
	// we are past the last key; just return that
	if (idx >= m_KeyCount - 1)
		return m_Keys[m_KeyCount - 1];

	// interpolate between the keys
	float a = (t - m_Times[idx]) / (m_Times[idx + 1] - m_Times[idx]);

	pix3 c = lerp(m_Keys[idx], m_Keys[idx + 1], a);
	return c;
}
