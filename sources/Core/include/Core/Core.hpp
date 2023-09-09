#pragma once

#include <memory>
#include <type_traits>
#include <fstream>

#define foreach2d(arr, y, x) \
  for(ptrdiff_t y = 0; y < arr.height(); ++y) \
    for(ptrdiff_t x = 0; x < arr.width(); ++x)

#define foreach_x(arr, x) \
  for(ptrdiff_t x = 0; x < arr.width(); ++x)

#define foreach_y(arr, y) \
  for(ptrdiff_t y = 0; y < arr.height(); ++y)

template<typename T>
class Array2d
{
public:
  Array2d() = default;
  explicit Array2d(ptrdiff_t h, ptrdiff_t w);

  Array2d(Array2d<T> const& other) = delete;
  Array2d(Array2d<T>&& other) = delete;

  Array2d& operator=(Array2d<T> other) = delete;
  Array2d& operator=(Array2d<T>&& other) = delete;

  ptrdiff_t width() const;
  ptrdiff_t height() const;

  T* data();
  T const * data() const;

  T& operator()(ptrdiff_t y, ptrdiff_t x);
  T const& operator()(ptrdiff_t y, ptrdiff_t x) const;

  void alloc(ptrdiff_t h, ptrdiff_t w);

private:
  ptrdiff_t h_ = 0;
  ptrdiff_t w_ = 0;
  std::unique_ptr<T[]> data_ = nullptr;
};

template<typename T>
inline Array2d<T>::Array2d(ptrdiff_t h, ptrdiff_t w) : h_(h), w_(w), data_(std::make_unique<T[]>(h * w))
{
  static_assert(std::is_arithmetic_v<T>, "Array2d template type needs to be of arithmetic type.");
}

template<typename T>
inline ptrdiff_t Array2d<T>::width() const
{
  return w_;
}

template<typename T>
inline ptrdiff_t Array2d<T>::height() const
{
  return h_;
}

template<typename T>
inline T* Array2d<T>::data()
{
  return data_.get();
}

template<typename T>
inline T const* Array2d<T>::data() const
{
  return data_.get();
}

template<typename T>
inline T& Array2d<T>::operator()(ptrdiff_t y, ptrdiff_t x)
{
  return data_[y * w_ + x];
}

template<typename T>
inline T const& Array2d<T>::operator()(ptrdiff_t y, ptrdiff_t x) const
{
  return data_[y * w_ + x];
}

template<typename T>
inline void Array2d<T>::alloc(ptrdiff_t h, ptrdiff_t w)
{
  h_ = h;
  w_ = w;
  data_ = std::make_unique<T[]>(h * w);
}

template<typename T>
void fill(Array2d<T>& arr, T val)
{
  foreach2d(arr, y, x)
  {
    arr(y, x) = val;
  }
}

template<typename T, typename U>
void fill(Array2d<T>& arr, Array2d<U> const& other)
{
  foreach2d(arr, y, x)
  {
    arr(y, x) = static_cast<T>(other(y, x));
  }
}

template<typename T>
void add(Array2d<T> const& src1, T src2, Array2d<T>& dst)
{
  foreach2d(dst, y, x)
    dst(y, x) = src1(y, x) + src2;
}

template<typename T>
void add(Array2d<T> const& src1, Array2d<T> const& src2, Array2d<T>& dst)
{
  foreach2d(dst, y, x)
    dst(y, x) = src1(y, x) + src2(y, x);
}

template<typename T>
void sub(Array2d<T> const& src1, T src2, Array2d<T>& dst)
{
  foreach2d(dst, y, x)
    dst(y, x) = src1(y, x) - src2;
}

template<typename T>
void sub(Array2d<T> const& src1, Array2d<T> const& src2, Array2d<T>& dst)
{
  foreach2d(dst, y, x)
    dst(y, x) = src1(y, x) - src2(y, x);
}

template<typename T>
void mul(Array2d<T> const& src1, T src2, Array2d<T>& dst)
{
  foreach2d(dst, y, x)
    dst(y, x) = src1(y, x) * src2;
}

template<typename T>
void mul(Array2d<T> const& src1, Array2d<T> const& src2, Array2d<T>& dst)
{
  foreach2d(dst, y, x)
    dst(y, x) = src1(y, x) * src2(y, x);
}

template<typename T>
T sum(Array2d<T> const& src)
{
  T sum = T(0);
  foreach2d(src, y, x)
    sum += src(y, x);

  return sum;
}

template<typename T>
T min_value(Array2d<T> const& src)
{
  auto min = src(0, 0);
  foreach2d(src, y, x)
    min = src(y, x) < min ? src(y, x) : min;

  return min;
}

template<typename T>
T max_value(Array2d<T> const& src)
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
void filter_x(Array2d<T> const& src, T const* kernel, ptrdiff_t kernel_sz, BorderCondition bc, Array2d<T>& dst)
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
void filter_y(Array2d<T> const& src, T const* kernel, ptrdiff_t kernel_sz, BorderCondition bc, Array2d<T>& dst)
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
void diff_filter_x(Array2d<T> const& src, BorderCondition bc, Array2d<T>& dst)
{
  T diff_kernel[] = { T(-1), T(0), T(1) };
  filter_x(src, diff_kernel, 3, bc, dst);
}

template<typename T>
void diff_filter_y(Array2d<T> const& src, BorderCondition bc, Array2d<T>& dst)
{
  T diff_kernel[] = { T(-1), T(0), T(1) };
  filter_y(src, diff_kernel, 3, bc, dst);
}

template<typename T>
void sobel_x(Array2d<T> const& src, BorderCondition bc, Array2d<T>& dst)
{
  Array2d<T> tmp(src.height(), src.width());

  T avg_kernel[] = { T(1), T(2), T(1) };
  filter_x(src, avg_kernel, 3, bc, tmp);

  diff_filter_x(tmp, bc, dst);
}

template<typename T>
void sobel_y(Array2d<T> const& src, BorderCondition bc, Array2d<T>& dst)
{
  Array2d<T> tmp(src.height(), src.width());

  T avg_kernel[] = { T(1), T(2), T(1) };
  filter_y(src, avg_kernel, 3, bc, tmp);

  diff_filter_y(tmp, bc, dst);
}

template<typename T>
void box_filter_x(Array2d<T> const& src, ptrdiff_t kernel_radius, BorderCondition bc, Array2d<T>& dst)
{
  const auto kernel_sz = 2 * kernel_radius + 1;
  Array2d<T> box_kernel(1, kernel_sz);
  fill(box_kernel, T(1));

  filter_x(src, box_kernel.data(), kernel_sz, bc, dst);
}

template<typename T>
void box_filter_y(Array2d<T> const& src, ptrdiff_t kernel_radius, BorderCondition bc, Array2d<T>& dst)
{
  const auto kernel_sz = 2 * kernel_radius + 1;
  Array2d<T> box_kernel(1, kernel_sz);
  fill(box_kernel, T(1));

  filter_y(src, box_kernel.data(), kernel_sz, bc, dst);
}

template<typename T>
void box_filter(Array2d<T> const& src, ptrdiff_t kernel_radius_y, ptrdiff_t kernel_radius_x, BorderCondition bc, Array2d<T>& dst)
{
  Array2d<T> tmp(src.height(), src.width());
  box_filter_x(src, kernel_radius_x, bc, tmp);
  box_filter_y(tmp, kernel_radius_y, bc, dst);
}

namespace detail
{
  template<typename T>
  void create_gauss_kernel(ptrdiff_t kernel_radius, T sigma, Array2d<T>& dst)
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
void gauss_filter_x(Array2d<T> const& src, ptrdiff_t kernel_radius, T sigma, BorderCondition bc, Array2d<T>& dst)
{
  static_assert(std::is_floating_point_v<T>, "Gauss filter only supports floating point arrays.");

  Array2d<T> gauss_kernel;
  detail::create_gauss_kernel(kernel_radius, sigma, gauss_kernel);

  filter_x(src, gauss_kernel.data(), gauss_kernel.width(), bc, dst);
}

template<typename T>
void gauss_filter_y(Array2d<T> const& src, ptrdiff_t kernel_radius, T sigma, BorderCondition bc, Array2d<T>& dst)
{
  static_assert(std::is_floating_point_v<T>, "Gauss filter only supports floating point arrays.");

  Array2d<T> gauss_kernel;
  detail::create_gauss_kernel(kernel_radius, sigma, gauss_kernel);

  filter_y(src, gauss_kernel.data(), gauss_kernel.width(), bc, dst);
}

template<typename T>
void gauss_filter(Array2d<T> const& src, ptrdiff_t kernel_radius_y, ptrdiff_t kernel_radius_x, T sigma_y, T sigma_x, BorderCondition bc, Array2d<T>& dst)
{
  Array2d<T> tmp(src.height(), src.width());
  gauss_filter_x(src, kernel_radius_x, sigma_x, bc, tmp);
  gauss_filter_y(tmp, kernel_radius_y, sigma_y, bc, dst);
}

