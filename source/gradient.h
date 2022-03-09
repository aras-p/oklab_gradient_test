#pragma once

#include "mathlib.h"

struct Gradient
{
	static constexpr int kMaxKeys = 8;
	pix3 m_Keys[kMaxKeys];
	float m_Times[kMaxKeys];
	float m_InvTimeDeltas[kMaxKeys]; // 1/(t1-t0) between each key
	int m_KeyCount = 0;

	void Precalc();
	pix3 Evaluate_sRGB(float t) const;
	pix3 Evaluate_Linear(float t) const;
	pix3 Evaluate_OkLab(float t) const;
};
