#pragma once

#include <stddef.h>
#include <variant>

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

struct GradConfig
{
  enum class GradType { GradX, GradY, GradAbs };

  GradType type;
};

struct CannyConfig
{
  float lo_thresh;
  float hi_thresh;
};

using OpConfig = std::variant<ThresholdConfig, FilterConfig, GradConfig, CannyConfig>;
