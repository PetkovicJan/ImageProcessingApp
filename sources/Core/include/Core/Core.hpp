#pragma once

#include <memory>
#include <type_traits>

template<typename T>
class Array2d
{
public:
  explicit Array2d(size_t h, size_t w);

  size_t width() const;
  size_t height() const;

  T* data();
  T const * data() const;

  T& operator()(size_t y, size_t x);

  T const& operator()(size_t y, size_t x) const;

private:
  size_t h_;
  size_t w_;
  std::unique_ptr<T[]> data_ = nullptr;
};

template<typename T>
inline Array2d<T>::Array2d(size_t h, size_t w) : h_(h), w_(w), data_(new T[h * w])
{
  static_assert(std::is_arithmetic_v<T>, "Array2d template type needs to be of arithmetic type.");
}

template<typename T>
inline size_t Array2d<T>::width() const
{
  return w_;
}

template<typename T>
inline size_t Array2d<T>::height() const
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
inline T& Array2d<T>::operator()(size_t y, size_t x)
{
  return data_[y * w_ + x];
}

template<typename T>
inline T const& Array2d<T>::operator()(size_t y, size_t x) const
{
  return data_[y * w_ + x];
}


