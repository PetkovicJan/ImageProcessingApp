#pragma once

#include <Core/OpConfigDefines.hpp>

#include <memory>
#include <type_traits>
#include <fstream>
#include <vector>

#define foreach2d(img, y, x) \
  for(ptrdiff_t y = 0; y < img.height(); ++y) \
    for(ptrdiff_t x = 0; x < img.width(); ++x)

#define foreach_x(img, x) \
  for(ptrdiff_t x = 0; x < img.width(); ++x)

#define foreach_y(img, y) \
  for(ptrdiff_t y = 0; y < img.height(); ++y)

struct Position
{
  explicit Position(ptrdiff_t y, ptrdiff_t x) : y(y), x(x) {}

  ptrdiff_t y;
  ptrdiff_t x;

  Position operator+(Position const& other) const
  {
    Position res = *this;
    res.x += other.x;
    res.y += other.y;

    return res;
  }

  Position operator-(Position const& other) const
  {
    Position res = *this;
    res.x -= other.x;
    res.y -= other.y;

    return res;
  }
};

namespace detail
{
  bool is_in_range(ptrdiff_t i, ptrdiff_t lo, ptrdiff_t hi);
}

template<typename T>
class Image2d
{
public:
  Image2d() = default;
  explicit Image2d(ptrdiff_t h, ptrdiff_t w);
  explicit Image2d(Position const& sz);

  Image2d(Image2d<T> const& other) = delete;
  Image2d(Image2d<T>&& other);

  Image2d& operator=(Image2d<T> const& other) = delete;
  Image2d& operator=(Image2d<T>&& other);

  ptrdiff_t width() const;
  ptrdiff_t height() const;
  Position size() const;

  T* data();
  T const * data() const;

  T& operator()(ptrdiff_t y, ptrdiff_t x);
  T const& operator()(ptrdiff_t y, ptrdiff_t x) const;

  T& operator()(Position pos);
  T const& operator()(Position pos) const;

  void alloc(ptrdiff_t h, ptrdiff_t w);
  void alloc(Position const& sz);

  bool isValid(ptrdiff_t y, ptrdiff_t x) const
  {
    return detail::is_in_range(y, 0, h_) && detail::is_in_range(x, 0, w_);
  }

  bool isValid(Position pos) const
  {
    return isValid(pos.y, pos.x);
  }

private:
  ptrdiff_t h_ = 0;
  ptrdiff_t w_ = 0;
  std::unique_ptr<T[]> data_ = nullptr;
};

template<typename T>
inline Image2d<T>::Image2d(ptrdiff_t h, ptrdiff_t w) : h_(h), w_(w), data_(std::make_unique<T[]>(h * w))
{
  static_assert(std::is_arithmetic_v<T>, "Image2d template type needs to be of arithmetic type.");
}

template<typename T>
inline Image2d<T>::Image2d(Position const& sz) : Image2d<T>(sz.y, sz.x) {}

template<typename T>
inline Image2d<T>::Image2d(Image2d<T>&& other)
{
  *this = std::move(other);
}

template<typename T>
inline Image2d<T>& Image2d<T>::operator=(Image2d<T>&& other)
{
  h_ = other.h_;
  w_ = other.w_;
  data_ = std::move(other.data_);

  other.h_ = 0;
  other.w_ = 0;

  return *this;
}

template<typename T>
inline ptrdiff_t Image2d<T>::width() const
{
  return w_;
}

template<typename T>
inline ptrdiff_t Image2d<T>::height() const
{
  return h_;
}

template<typename T>
inline Position Image2d<T>::size() const
{
  return Position(h_, w_);
}

template<typename T>
inline T* Image2d<T>::data()
{
  return data_.get();
}

template<typename T>
inline T const* Image2d<T>::data() const
{
  return data_.get();
}

template<typename T>
inline T& Image2d<T>::operator()(ptrdiff_t y, ptrdiff_t x)
{
  return data_[y * w_ + x];
}

template<typename T>
inline T const& Image2d<T>::operator()(ptrdiff_t y, ptrdiff_t x) const
{
  return data_[y * w_ + x];
}

template<typename T>
inline T& Image2d<T>::operator()(Position pos)
{
  return data_[pos.y * w_ + pos.x];
}

template<typename T>
inline T const& Image2d<T>::operator()(Position pos) const
{
  return data_[pos.y * w_ + pos.x];
}

template<typename T>
inline void Image2d<T>::alloc(ptrdiff_t h, ptrdiff_t w)
{
  h_ = h;
  w_ = w;
  data_ = std::make_unique<T[]>(h * w);
}

template<typename T>
inline void Image2d<T>::alloc(Position const& sz)
{
  alloc(sz.y, sz.x);
}

template<typename T>
void fill(Image2d<T>& img, T val)
{
  foreach2d(img, y, x)
  {
    img(y, x) = val;
  }
}

template<typename T, typename U>
void fill(Image2d<T>& img, Image2d<U> const& other)
{
  foreach2d(img, y, x)
  {
    img(y, x) = static_cast<T>(other(y, x));
  }
}

template<typename T>
void add(Image2d<T> const& src1, T src2, Image2d<T>& dst)
{
  foreach2d(dst, y, x)
    dst(y, x) = src1(y, x) + src2;
}

template<typename T>
void add(Image2d<T> const& src1, Image2d<T> const& src2, Image2d<T>& dst)
{
  foreach2d(dst, y, x)
    dst(y, x) = src1(y, x) + src2(y, x);
}

template<typename T>
void sub(Image2d<T> const& src1, T src2, Image2d<T>& dst)
{
  foreach2d(dst, y, x)
    dst(y, x) = src1(y, x) - src2;
}

template<typename T>
void sub(Image2d<T> const& src1, Image2d<T> const& src2, Image2d<T>& dst)
{
  foreach2d(dst, y, x)
    dst(y, x) = src1(y, x) - src2(y, x);
}

template<typename T>
void mul(Image2d<T> const& src1, T src2, Image2d<T>& dst)
{
  foreach2d(dst, y, x)
    dst(y, x) = src1(y, x) * src2;
}

template<typename T>
void mul(Image2d<T> const& src1, Image2d<T> const& src2, Image2d<T>& dst)
{
  foreach2d(dst, y, x)
    dst(y, x) = src1(y, x) * src2(y, x);
}

template<typename T>
T sum(Image2d<T> const& src)
{
  T sum = T(0);
  foreach2d(src, y, x)
    sum += src(y, x);

  return sum;
}

template<typename T>
T min_value(Image2d<T> const& src)
{
  auto min = src(0, 0);
  foreach2d(src, y, x)
    min = src(y, x) < min ? src(y, x) : min;

  return min;
}

template<typename T>
T max_value(Image2d<T> const& src)
{
  auto max = src(0, 0);
  foreach2d(src, y, x)
    max = src(y, x) > max ? src(y, x) : max;

  return max;
}

enum class BorderCondition {
  BC_ZERO,
  BC_CLAMP,
  BC_WRAP
};

template<typename T>
void filter_x(Image2d<T> const& src, T const* kernel, ptrdiff_t kernel_sz, BorderCondition bc, Image2d<T>& dst)
{
  // Handle inner pixels (avoiding boundary conditions).
  const ptrdiff_t kernel_radius = kernel_sz / 2;
  for(ptrdiff_t i = 0; i < src.height(); ++i)
    for (ptrdiff_t j = kernel_radius; j < src.width() - kernel_radius; ++j)
    {
      const ptrdiff_t j_begin = j - kernel_radius;
      T filtered = T(0);
      for(ptrdiff_t ik = 0; ik < kernel_sz; ++ik)
        filtered += kernel[ik] * src(i, j_begin + ik);

      dst(i, j) = filtered;
    }

  // Handle border pixels.
  switch (bc)
  {
  case BorderCondition::BC_ZERO:
  {
    // Left border pixels.
    for(ptrdiff_t i = 0; i < src.height(); ++i)
      for (ptrdiff_t j = 0; j < kernel_radius; ++j)
      {
        const auto j_begin = j - kernel_radius;
        const auto k_begin = -j_begin;

        T filtered = T(0);
        for(ptrdiff_t ik = k_begin; ik < kernel_sz; ++ik)
          filtered += kernel[ik] * src(i, j_begin + ik);

        dst(i, j) = filtered;
      }

    // Right border pixels.
    for(ptrdiff_t i = 0; i < src.height(); ++i)
      for (ptrdiff_t j = src.width() - kernel_radius; j < src.width(); ++j)
      {
        const auto j_begin = j - kernel_radius;
        const auto k_end = src.width() - j_begin;

        T filtered = T(0);
        for(ptrdiff_t ik = 0; ik < k_end; ++ik)
          filtered += kernel[ik] * src(i, j_begin + ik);

        dst(i, j) = filtered;
      }
  }
  break;
  case BorderCondition::BC_CLAMP:
  {
    // Left border pixels.
    for(ptrdiff_t i = 0; i < src.height(); ++i)
      for (ptrdiff_t j = 0; j < kernel_radius; ++j)
      {
        const auto j_begin = j - kernel_radius;
        const auto k_begin = -j_begin;

        T filtered = T(0);
        for(ptrdiff_t ik = k_begin; ik < kernel_sz; ++ik)
          filtered += kernel[ik] * src(i, j_begin + ik);

        for (ptrdiff_t ik = 0; ik < k_begin; ++ik)
          filtered += kernel[ik] * src(i, 0);

        dst(i, j) = filtered;
      }

    // Right border pixels.
    for(ptrdiff_t i = 0; i < src.height(); ++i)
      for (ptrdiff_t j = src.width() - kernel_radius; j < src.width(); ++j)
      {
        const auto j_begin = j - kernel_radius;
        const auto k_end = src.width() - j_begin;

        T filtered = T(0);
        for(ptrdiff_t ik = 0; ik < k_end; ++ik)
          filtered += kernel[ik] * src(i, j_begin + ik);

        for (ptrdiff_t ik = k_end; ik < kernel_sz; ++ik)
          filtered += kernel[ik] * src(i, src.width() - 1);

        dst(i, j) = filtered;
      }
  }
  break;
  case BorderCondition::BC_WRAP:
  {
    // Left border pixels.
    for(ptrdiff_t i = 0; i < src.height(); ++i)
      for (ptrdiff_t j = 0; j < kernel_radius; ++j)
      {
        const auto j_begin = j - kernel_radius;
        const auto k_begin = -j_begin;

        T filtered = T(0);
        for(ptrdiff_t ik = k_begin; ik < kernel_sz; ++ik)
          filtered += kernel[ik] * src(i, j_begin + ik);

        const auto wrap_begin = src.width() - k_begin;
        for (ptrdiff_t ik = 0; ik < k_begin; ++ik)
          filtered += kernel[ik] * src(i, wrap_begin + ik);

        dst(i, j) = filtered;
      }

    // Right border pixels.
    for(ptrdiff_t i = 0; i < src.height(); ++i)
      for (ptrdiff_t j = src.width() - kernel_radius; j < src.width(); ++j)
      {
        const auto j_begin = j - kernel_radius;
        const auto k_end = src.width() - j_begin;

        T filtered = T(0);
        for(ptrdiff_t ik = 0; ik < k_end; ++ik)
          filtered += kernel[ik] * src(i, j_begin + ik);

        for (ptrdiff_t ik = k_end; ik < kernel_sz; ++ik)
          filtered += kernel[ik] * src(i, ik - k_end);

        dst(i, j) = filtered;
      }
  }
  break;
  }
}

template<typename T>
void filter_y(Image2d<T> const& src, T const* kernel, ptrdiff_t kernel_sz, BorderCondition bc, Image2d<T>& dst)
{
  const ptrdiff_t kernel_radius = kernel_sz / 2;
  for(ptrdiff_t i = kernel_radius; i < src.height() - kernel_radius; ++i)
    for (ptrdiff_t j = 0; j < src.width(); ++j)
    {
      const ptrdiff_t i_begin = i - kernel_radius;
      T filtered = T(0);
      for(ptrdiff_t ik = 0; ik < kernel_sz; ++ik)
        filtered += kernel[ik] * src(i_begin + ik, j);

      dst(i, j) = filtered;
    }

  // Handle border pixels.
  switch (bc)
  {
  case BorderCondition::BC_ZERO:
  {
    // Top border pixels.
    for(ptrdiff_t i = 0; i < kernel_radius; ++i)
      for (ptrdiff_t j = 0; j < src.width(); ++j)
      {
        const auto i_begin = i - kernel_radius;
        const auto k_begin = -i_begin;

        T filtered = T(0);
        for(ptrdiff_t ik = k_begin; ik < kernel_sz; ++ik)
          filtered += kernel[ik] * src(i_begin + ik, j);

        dst(i, j) = filtered;
      }

    // Bottom border pixels.
    for(ptrdiff_t i = src.height() - kernel_radius; i < src.height(); ++i)
      for (ptrdiff_t j = 0; j < src.width(); ++j)
      {
        const auto i_begin = i - kernel_radius;
        const auto k_end = src.height() - i_begin;

        T filtered = T(0);
        for(ptrdiff_t ik = 0; ik < k_end; ++ik)
          filtered += kernel[ik] * src(i_begin + ik, j);

        dst(i, j) = filtered;
      }
  }
  break;
  case BorderCondition::BC_CLAMP:
  {
    // Top border pixels.
    for(ptrdiff_t i = 0; i < kernel_radius; ++i)
      for (ptrdiff_t j = 0; j < src.width(); ++j)
      {
        const auto i_begin = i - kernel_radius;
        const auto k_begin = -i_begin;

        T filtered = T(0);
        for(ptrdiff_t ik = k_begin; ik < kernel_sz; ++ik)
          filtered += kernel[ik] * src(i_begin + ik, j);

        for (ptrdiff_t ik = 0; ik < k_begin; ++ik)
          filtered += kernel[ik] * src(0, j);

        dst(i, j) = filtered;
      }

    // Bottom border pixels.
    for(ptrdiff_t i = src.height() - kernel_radius; i < src.height(); ++i)
      for (ptrdiff_t j = 0; j < src.width(); ++j)
      {
        const auto i_begin = i - kernel_radius;
        const auto k_end = src.height() - i_begin;

        T filtered = T(0);
        for(ptrdiff_t ik = 0; ik < k_end; ++ik)
          filtered += kernel[ik] * src(i_begin + ik, j);

        for (ptrdiff_t ik = k_end; ik < kernel_sz; ++ik)
          filtered += kernel[ik] * src(src.height() - 1, j);

        dst(i, j) = filtered;
      }
  }
  break;
  case BorderCondition::BC_WRAP:
  {
    // Top border pixels.
    for(ptrdiff_t i = 0; i < kernel_radius; ++i)
      for (ptrdiff_t j = 0; j < src.width(); ++j)
      {
        const auto i_begin = i - kernel_radius;
        const auto k_begin = -i_begin;

        T filtered = T(0);
        for(ptrdiff_t ik = k_begin; ik < kernel_sz; ++ik)
          filtered += kernel[ik] * src(i_begin + ik, j);

        const auto wrap_begin = src.height() - k_begin;
        for (ptrdiff_t ik = 0; ik < k_begin; ++ik)
          filtered += kernel[ik] * src(wrap_begin + ik, j);

        dst(i, j) = filtered;
      }

    // Bottom border pixels.
    for(ptrdiff_t i = src.height() - kernel_radius; i < src.height(); ++i)
      for (ptrdiff_t j = 0; j < src.width(); ++j)
      {
        const auto i_begin = i - kernel_radius;
        const auto k_end = src.height() - i_begin;

        T filtered = T(0);
        for(ptrdiff_t ik = 0; ik < k_end; ++ik)
          filtered += kernel[ik] * src(i_begin + ik, j);

        for (ptrdiff_t ik = k_end; ik < kernel_sz; ++ik)
          filtered += kernel[ik] * src(ik - k_end, j);

        dst(i, j) = filtered;
      }
  }
  break;
  }
}

template<typename T>
void diff_filter_x(Image2d<T> const& src, BorderCondition bc, Image2d<T>& dst)
{
  T diff_kernel[] = { T(-1), T(0), T(1) };
  filter_x(src, diff_kernel, 3, bc, dst);
}

template<typename T>
void diff_filter_y(Image2d<T> const& src, BorderCondition bc, Image2d<T>& dst)
{
  T diff_kernel[] = { T(-1), T(0), T(1) };
  filter_y(src, diff_kernel, 3, bc, dst);
}

template<typename T>
void sobel_x(Image2d<T> const& src, BorderCondition bc, Image2d<T>& dst)
{
  Image2d<T> tmp(src.height(), src.width());

  T avg_kernel[] = { T(1), T(2), T(1) };
  filter_x(src, avg_kernel, 3, bc, tmp);

  diff_filter_x(tmp, bc, dst);
}

template<typename T>
void sobel_y(Image2d<T> const& src, BorderCondition bc, Image2d<T>& dst)
{
  Image2d<T> tmp(src.height(), src.width());

  T avg_kernel[] = { T(1), T(2), T(1) };
  filter_y(src, avg_kernel, 3, bc, tmp);

  diff_filter_y(tmp, bc, dst);
}

template<typename T>
void box_filter_x(Image2d<T> const& src, ptrdiff_t kernel_radius, BorderCondition bc, Image2d<T>& dst)
{
  const auto kernel_sz = 2 * kernel_radius + 1;
  Image2d<T> box_kernel(1, kernel_sz);
  fill(box_kernel, T(1));

  filter_x(src, box_kernel.data(), kernel_sz, bc, dst);
}

template<typename T>
void box_filter_y(Image2d<T> const& src, ptrdiff_t kernel_radius, BorderCondition bc, Image2d<T>& dst)
{
  const auto kernel_sz = 2 * kernel_radius + 1;
  Image2d<T> box_kernel(1, kernel_sz);
  fill(box_kernel, T(1));

  filter_y(src, box_kernel.data(), kernel_sz, bc, dst);
}

template<typename T>
void box_filter(Image2d<T> const& src, ptrdiff_t kernel_radius_y, ptrdiff_t kernel_radius_x, BorderCondition bc, Image2d<T>& dst)
{
  Image2d<T> tmp(src.height(), src.width());
  box_filter_x(src, kernel_radius_x, bc, tmp);
  box_filter_y(tmp, kernel_radius_y, bc, dst);
}

namespace detail
{
  template<typename T>
  void create_gauss_kernel(ptrdiff_t kernel_radius, T sigma, Image2d<T>& dst)
  {
    const auto kernel_sz = 2 * kernel_radius + 1;
    const auto factor = T(0.5) / (sigma * sigma);
    dst.alloc(1, kernel_sz);
    foreach_x(dst, x)
    {
      const auto dx = x - kernel_radius;
      dst(0, x) = exp(-factor * T(dx * dx));
    }

    // Normalize the kernel.
    const auto normalization = T(1) / sum(dst);
    mul(dst, normalization, dst);
  }
}

template<typename T>
void gauss_filter_x(Image2d<T> const& src, ptrdiff_t kernel_radius, T sigma, BorderCondition bc, Image2d<T>& dst)
{
  static_assert(std::is_floating_point_v<T>, "Gauss filter only supports floating point images.");

  Image2d<T> gauss_kernel;
  detail::create_gauss_kernel(kernel_radius, sigma, gauss_kernel);

  filter_x(src, gauss_kernel.data(), gauss_kernel.width(), bc, dst);
}

template<typename T>
void gauss_filter_y(Image2d<T> const& src, ptrdiff_t kernel_radius, T sigma, BorderCondition bc, Image2d<T>& dst)
{
  static_assert(std::is_floating_point_v<T>, "Gauss filter only supports floating point images.");

  Image2d<T> gauss_kernel;
  detail::create_gauss_kernel(kernel_radius, sigma, gauss_kernel);

  filter_y(src, gauss_kernel.data(), gauss_kernel.width(), bc, dst);
}

template<typename T>
void gauss_filter(Image2d<T> const& src, ptrdiff_t kernel_radius_y, ptrdiff_t kernel_radius_x, T sigma_y, T sigma_x, BorderCondition bc, Image2d<T>& dst)
{
  Image2d<T> tmp(src.height(), src.width());
  gauss_filter_x(src, kernel_radius_x, sigma_x, bc, tmp);
  gauss_filter_y(tmp, kernel_radius_y, sigma_y, bc, dst);
}

template<typename T, typename U>
void threshold_image(Image2d<T> const& src, T threshold, U true_val, U false_val, Image2d<U>& dst)
{
  foreach2d(src, y, x)
  {
    dst(y, x) = src(y, x) >= threshold ? true_val : false_val;
  }
}

template<typename T>
void erode(Image2d<T> const& src, ptrdiff_t kernel_radius, Image2d<T>& dst)
{
  static_assert(std::is_integral_v<T>, "Erosion only supports integral images.");

  const auto kernel_sz = 2 * kernel_radius + 1;

  for (ptrdiff_t i = 0; i < src.height(); ++i)
  {
    if (i < kernel_radius || i >= (src.height() - kernel_radius))
    {
      for (ptrdiff_t j = 0; j < src.width(); ++j)
        dst(i, j) = T(0);
    }

    for (ptrdiff_t j = 0; j < src.width(); ++j)
    {
      if (j < kernel_radius || j >= (src.width() - kernel_radius))
      {
        dst(i, j) = T(0);
        continue;
      }

      ptrdiff_t sum = 0;
      for(ptrdiff_t ik = - kernel_radius; ik <= kernel_radius; ++ik)
        for (ptrdiff_t jk = -kernel_radius; jk <= kernel_radius; ++jk)
        {
          if (src(i + ik, j + jk))
            ++sum;
        }

      if (sum == kernel_sz * kernel_sz)
        dst(i, j) = T(1);
      else
        dst(i, j) = T(0);
    }
  }
}

namespace constants
{
  constexpr const double pi = 3.14159265359;
  constexpr const double two_pi = pi * 2.;
  constexpr const double half_pi = pi / 2.;
  constexpr const double quarter_pi = pi / 4.;
  constexpr const double eighth_pi = pi / 8.;
}

namespace detail
{
  ptrdiff_t clamp_index(ptrdiff_t index, ptrdiff_t size);

  int8_t canny_get_angle_bin(double angle);

  template<typename T>
  void non_max_suppression(Image2d<T> const& grad, Image2d<int8_t> const& dirs, Image2d<unsigned char>& dst)
  {
    const auto w = grad.width();
    const auto h = grad.height();

    foreach2d(grad, y, x)
    {
      auto prev_x = x;
      auto prev_y = y;

      auto next_x = x;
      auto next_y = y;

      const auto dir = dirs(y, x);
      switch (dir)
      {
      case 0:
      {
        prev_x = x - 1;
        next_x = x + 1;
      }
      break;
      case 1:
      {
        prev_x = x - 1;
        prev_y = y - 1;

        next_x = x + 1;
        next_y = y + 1;
      }
      break;
      case 2:
      {
        prev_y = y - 1;
        next_y = y + 1;
      }
      break;
      case 3:
      {
        prev_x = x + 1;
        prev_y = y - 1;

        next_x = x - 1;
        next_y = y + 1;
      }
      break;
      }

      // Clamp indices.
      prev_x = clamp_index(prev_x, w);
      prev_y = clamp_index(prev_y, h);

      next_x = clamp_index(next_x, w);
      next_y = clamp_index(next_y, h);
      
      // Non-max suppression.
      const auto curr_val = grad(y, x);
      const auto prev_val = grad(prev_y, prev_x);
      const auto next_val = grad(next_y, next_x);
      if (curr_val > prev_val && curr_val >= next_val)
      {
        dst(y, x) = 1;
      }
      else
      {
        dst(y, x) = 0;
      }
    }
  }

  void hysteresis_edge_tracking(Image2d<unsigned char>& dst);
}

template<typename T>
void canny_edge_detection(Image2d<T> const& src, T low_threshold, T high_threshold, Image2d<unsigned char>& dst)
{
  const auto w = src.width();
  const auto h = src.height();

  Image2d<T> grad_x(h, w);
  sobel_x(src, BorderCondition::BC_CLAMP, grad_x);

  Image2d<T> grad_y(h, w);
  sobel_y(src, BorderCondition::BC_CLAMP, grad_y);

  Image2d<T> grad_sq(h, w);
  Image2d<int8_t> directions(h, w);
  foreach2d(src, y, x)
  {
    const auto gx = grad_x(y, x);
    const auto gy = grad_y(y, x);

    grad_sq(y, x) = gx * gx + gy * gy;

    const double angle = (gx == T(0)) ? constants::half_pi : atan2(gy, gx);
    directions(y, x) = detail::canny_get_angle_bin(angle);
  }

  detail::non_max_suppression(grad_sq, directions, dst);

  // Threshold application.
  const auto lo_sq = low_threshold * low_threshold;
  const auto hi_sq = high_threshold * high_threshold;
  foreach2d(dst, y, x)
  {
    if (dst(y, x) == 1)
    {
      const auto val_sq = grad_sq(y, x);
      dst(y, x) = val_sq < lo_sq ? 0 : (val_sq < hi_sq ? 1 : 2);
    }
  }

  detail::hysteresis_edge_tracking(dst);
}

template<typename T>
void export_image(std::string const& file_name, Image2d<T>& img)
{
  std::fstream file(file_name, std::ios::out);

  const auto min = min_value(img);
  const auto max = max_value(img);
  const auto scale = 255.999 / double(max - min + T(1));

  file << "P3\n" << img.width() << ' ' << img.height() << "\n255\n";

  foreach2d(img, y, x)
  {
    const auto val = int(double(img(y, x) - min) * scale);
    file << val << ' ' << val << ' ' << val << '\n';
  }
 
  file.close();
}

class Operation
{
public:
  virtual ~Operation() {}

  virtual void perform(Image2d<float> const& in, Image2d<float>& out) const = 0;
};

class ThresholdOp : public Operation
{
public:
  ThresholdOp(ThresholdConfig const& config); 

  void perform(Image2d<float> const& in, Image2d<float>& out) const override;

private:
  ThresholdConfig config_;
};

class FilterOp : public Operation
{
public:
  FilterOp(FilterConfig const& config);

  void perform(Image2d<float> const& in, Image2d<float>& out) const override;

private:
  FilterConfig config_;
};

class OperationChain
{
public:
  void addOperation(int op_id, OpConfig const& config);
  void modifyOperation(int op_id, OpConfig const& config);
  void removeOperation(int op_id);

  void executeChain(Image2d<float> const& in, Image2d<float>& out) const;

private:
  // Chain of operations with corresponding unique IDs.
  std::vector<std::pair<int, std::unique_ptr<Operation>>> chain_;
};
