#pragma once

#include "mathlib.h"

float3 Linear_sRGB_to_OkLab(float3 c);
float3 OkLab_to_Linear_sRGB(float3 c);

float3 Linear_sRGB_to_LMS(float3 c);
float3 LMS_to_Linear_sRGB(float3 c);
