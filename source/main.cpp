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

static void MeasureTime(int seed, const char *name, eval_func func, const int expSum[3])
{
	uint32_t state = seed;
	int sum[3] = {};
	auto t0 = std::chrono::steady_clock::now();
	int allowedDiff = 2;
	for (int i = 0; i < 10000000; ++i)
	{
		float a = RandomFloat01(state);
		pix3 c = func(a);
		//printf("%.4f: %i, %i, %i\n", a, c.r, c.g, c.b);
		sum[0] += c.r;
		sum[1] += c.g;
		sum[2] += c.b;
	}
	auto t1 = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> ms = t1 - t0;
	printf("%-15s %.1fms, sum %i, %i, %i\n", name, ms.count(), sum[0], sum[1], sum[2]);
	if (abs(expSum[0] - sum[0]) > allowedDiff)
	{
		printf("  FAILED: expected R sum to be %i, got %i\n", expSum[0], sum[0]);
		exit(1);
	}
	if (abs(expSum[1] - sum[1]) > allowedDiff)
	{
		printf("  FAILED: expected G sum to be %i, got %i\n", expSum[1], sum[1]);
		exit(1);
	}
	if (abs(expSum[2] - sum[2]) > allowedDiff)
	{
		printf("  FAILED: expected B sum to be %i, got %i\n", expSum[2], sum[2]);
		exit(1);
	}
}

static pix3 Eval_sRGB(float a)
{
	return s_Gradient.Evaluate_sRGB(a);
}

static pix3 Eval_Linear(float a)
{
	return s_Gradient.Evaluate_Linear(a);
}

static pix3 Eval_OkLab(float a)
{
	return s_Gradient.Evaluate_OkLab(a);
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

	int seed = argc + 1;
	int exp_sRGB[3] = { 1306914865, 1076623000, 816166200 };
	int exp_Linear[3] = { 1416453422, 1209420259, 917944709 };
	int exp_OkLab[3] = { 1396681593, 1183052365, 898569604 };
	MeasureTime(seed, "sRGB", Eval_sRGB, exp_sRGB);
	MeasureTime(seed, "Linear", Eval_Linear, exp_Linear);
	MeasureTime(seed, "OkLab", Eval_OkLab, exp_OkLab);
	return 0;
}
