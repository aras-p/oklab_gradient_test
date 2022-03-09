#include "oklab.h"
#include "gradient.h"
#include <chrono>
#include <stdio.h>

static uint32_t XorShift32(uint32_t& state)
{
	uint32_t x = state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 15;
	state = x;
	return x;
}

static float RandomFloat01(uint32_t& state)
{
	return (XorShift32(state) & 0xFFFFFF) / 16777216.0f;
}

static Gradient s_Gradient;

typedef pix3 (*eval_func)(float a);

static void MeasureTime(int seed, const char *name, eval_func func, int expSumR, int expSumG, int expSumB)
{
	uint32_t state = seed;
	int sum[3] = {};
	auto t0 = std::chrono::steady_clock::now();
	for (int i = 0; i < 10000000; ++i)
	{
		float a = RandomFloat01(state);
		pix3 c = func(a);
		sum[0] += c.r;
		sum[1] += c.g;
		sum[2] += c.b;
	}
	auto t1 = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> ms = t1 - t0;
	printf("%-15s %.1fms, sum %i,%i,%i\n", name, ms.count(), sum[0], sum[1], sum[2]);
	if (expSumR != sum[0])
	{
		printf("  FAILED: expected R sum to be %i, got %i\n", expSumR, sum[0]);
		exit(1);
	}
	if (expSumG != sum[1])
	{
		printf("  FAILED: expected G sum to be %i, got %i\n", expSumG, sum[1]);
		exit(1);
	}
	if (expSumB != sum[2])
	{
		printf("  FAILED: expected B sum to be %i, got %i\n", expSumB, sum[2]);
		exit(1);
	}
}

static pix3 EvalGradientRGB(float a)
{
	return s_Gradient.Evaluate(a);
}

int main(int argc, const char**)
{
	// "VIBGYOR" rainbow gradient
	s_Gradient.m_KeyCount = 7;
	s_Gradient.m_Keys[0] = { 148, 0, 211 };
	s_Gradient.m_Keys[1] = { 75, 0, 130 };
	s_Gradient.m_Keys[2] = { 0, 0, 255 };
	s_Gradient.m_Keys[3] = { 0, 255, 0 };
	s_Gradient.m_Keys[4] = { 255, 255, 0 };
	s_Gradient.m_Keys[5] = { 255, 127, 0 };
	s_Gradient.m_Keys[6] = { 255, 0, 0 };
	s_Gradient.m_Times[0] = 0.0f;
	s_Gradient.m_Times[1] = 0.17f;
	s_Gradient.m_Times[2] = 0.33f;
	s_Gradient.m_Times[3] = 0.5f;
	s_Gradient.m_Times[4] = 0.67f;
	s_Gradient.m_Times[5] = 0.84f;
	s_Gradient.m_Times[6] = 1.0f;
	s_Gradient.Precalc();

	int expRGBr = 1307146855, expRGBg = 1076488335, expRGBb = 816275141;
	MeasureTime(argc, "RGB", EvalGradientRGB, expRGBr, expRGBg, expRGBb);
	return 0;
}
