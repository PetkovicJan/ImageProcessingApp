#include <gtest/gtest.h>

#include <Core/Core.hpp>

TEST(Array2dBasicTest, ConstructionTest)
{
  const ptrdiff_t h = 20;
  const ptrdiff_t w = 10;
  Array2d<int> arr(h, w);
  ASSERT_NE(arr.data(), nullptr);
  ASSERT_EQ(arr.height(), h);
  ASSERT_EQ(arr.width(), w);
}

TEST(Array2dBasicTest, AccessArrayElements)
{
  const ptrdiff_t h = 20;
  const ptrdiff_t w = 10;
  Array2d<int> arr(h, w);
  for (ptrdiff_t i = 0; i < h; ++i)
    for (ptrdiff_t j = 0; j < w; ++j)
      arr(i, j) = i * w + j;

  for (ptrdiff_t i = 0; i < h; ++i)
    for (ptrdiff_t j = 0; j < w; ++j)
      ASSERT_EQ(arr(i, j), i * w + j);
}

TEST(Array2dBasicTest, Foreach2dLoopTest)
{
  const ptrdiff_t h = 20;
  const ptrdiff_t w = 10;
  Array2d<int> arr1(h, w);
  for (ptrdiff_t i = 0; i < h; ++i)
    for (ptrdiff_t j = 0; j < w; ++j)
      arr1(i, j) = i * w + j;

  Array2d<float> arr2(h, w);
  foreach2d(arr2, i, j)
    arr2(i, j) = i * w + j;

  for (ptrdiff_t i = 0; i < h; ++i)
    for (ptrdiff_t j = 0; j < w; ++j)
      ASSERT_EQ(arr1(i, j), arr2(i, j));
}

TEST(FilterFunctionTest, BoxFilterTest)
{
  const ptrdiff_t h = 20;
  const ptrdiff_t w = 10;
  Array2d<int> src(h, w);
  fill(src, 1);

  Array2d<int> filt(h, w);
  box_filter(src, 1, 1, BorderCondition::BC_ZERO, filt);
  
  // Check only inner pixels (border pixels are tested in a separate test).
  for (ptrdiff_t i = 1; i < h - 1; ++i)
    for (ptrdiff_t j = 1; j < w - 1; ++j)
      ASSERT_EQ(filt(i, j), 9);
}

TEST(FilterFunctionTest, TestZeroBoundaryCondition)
{
  const ptrdiff_t h = 20;
  const ptrdiff_t w = 10;
  Array2d<int> src(h, w);
  fill(src, 1);

  Array2d<int> filt(h, w);
  box_filter(src, 1, 1, BorderCondition::BC_ZERO, filt);
  
  // Check border pixels.
  for (ptrdiff_t i = 1; i < h - 1; ++i)
  {
    ASSERT_EQ(filt(i, 0), 6);
    ASSERT_EQ(filt(i, w - 1), 6);
  }

  for (ptrdiff_t j = 1; j < w - 1; ++j)
  {
    ASSERT_EQ(filt(0, j), 6);
    ASSERT_EQ(filt(h - 1, j), 6);
  }

  ASSERT_EQ(filt(0, 0), 4);
  ASSERT_EQ(filt(0, w - 1), 4);
  ASSERT_EQ(filt(h - 1, 0), 4);
  ASSERT_EQ(filt(h - 1, w - 1), 4);
}

TEST(FilterFunctionTest, TestClampBoundaryCondition)
{
  const ptrdiff_t h = 20;
  const ptrdiff_t w = 10;
  Array2d<int> src(h, w);
  fill(src, 1);

  Array2d<int> filt(h, w);
  box_filter(src, 1, 1, BorderCondition::BC_CLAMP, filt);
  
  // Check border pixels.
  for (ptrdiff_t i = 0; i < h; ++i)
  {
    ASSERT_EQ(filt(i, 0), 9);
    ASSERT_EQ(filt(i, w - 1), 9);
  }

  for (ptrdiff_t j = 0; j < w; ++j)
  {
    ASSERT_EQ(filt(0, j), 9);
    ASSERT_EQ(filt(h - 1, j), 9);
  }
}

TEST(FilterFunctionTest, TestwrapBoundaryConditionInXdir)
{
  const ptrdiff_t h = 20;
  const ptrdiff_t w = 10;
  Array2d<int> src(h, w);
  foreach2d(src, y, x)
  {
    src(y, x) = x < w / 2 ? 1 : 2;
  }

  Array2d<int> filt(h, w);
  box_filter_x(src, 1, BorderCondition::BC_WRAP, filt);
  
  // Check border pixels.
  for (ptrdiff_t i = 0; i < h; ++i)
  {
    ASSERT_EQ(filt(i, 0), 4);
    ASSERT_EQ(filt(i, w - 1), 5);
  }
}

TEST(FilterFunctionTest, TestwrapBoundaryConditionInYdir)
{
  const ptrdiff_t h = 20;
  const ptrdiff_t w = 10;
  Array2d<int> src(h, w);
  foreach2d(src, y, x)
  {
    src(y, x) = y < h / 2 ? 1 : 2;
  }

  Array2d<int> filt(h, w);
  box_filter_y(src, 1, BorderCondition::BC_WRAP, filt);
  
  // Check border pixels.
  for (ptrdiff_t j = 0; j < w; ++j)
  {
    ASSERT_EQ(filt(0, j), 4);
    ASSERT_EQ(filt(h - 1, j), 5);
  }
}

TEST(Array2dBasicTest, ExportArray)
{
  const ptrdiff_t h = 20;
  const ptrdiff_t w = 10;
  Array2d<int> arr(h, w);
  foreach2d(arr, i, j)
    arr(i, j) = j;

  Array2d<int> dst(h, w);
  // export_arr("test_arr_diff.ppm", dst);
}
