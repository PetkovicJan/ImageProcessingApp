#pragma once

#include<stddef.h>

struct ThresholdConfig
{
  float thresh;
  float true_val;
  float false_val;
};

struct FilterConfig
{
  ptrdiff_t kernel_radius_x;
  ptrdiff_t kernel_radius_y;

  float sigma_x;
  float sigma_y;
};
