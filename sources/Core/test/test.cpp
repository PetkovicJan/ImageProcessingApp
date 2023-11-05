#include <gtest/gtest.h>

#include <Core/Core.hpp>

TEST(Image2dBasicTest, ConstructionTest)
{
  const ptrdiff_t h = 20;
  const ptrdiff_t w = 10;
  Image2d<int> img(h, w);
  ASSERT_NE(img.data(), nullptr);
  ASSERT_EQ(img.height(), h);
  ASSERT_EQ(img.width(), w);
}

TEST(Image2dBasicTest, AccessArrayElements)
{
  const ptrdiff_t h = 20;
  const ptrdiff_t w = 10;
  Image2d<int> img(h, w);
  for (ptrdiff_t i = 0; i < h; ++i)
    for (ptrdiff_t j = 0; j < w; ++j)
      img(i, j) = i * w + j;

  for (ptrdiff_t i = 0; i < h; ++i)
    for (ptrdiff_t j = 0; j < w; ++j)
      ASSERT_EQ(img(i, j), i * w + j);
}

TEST(Image2dBasicTest, Foreach2dLoopTest)
{
  const ptrdiff_t h = 20;
  const ptrdiff_t w = 10;
  Image2d<int> img1(h, w);
  for (ptrdiff_t i = 0; i < h; ++i)
    for (ptrdiff_t j = 0; j < w; ++j)
      img1(i, j) = i * w + j;

  Image2d<float> img2(h, w);
  foreach2d(img2, i, j)
    img2(i, j) = i * w + j;

  for (ptrdiff_t i = 0; i < h; ++i)
    for (ptrdiff_t j = 0; j < w; ++j)
      ASSERT_EQ(img1(i, j), img2(i, j));
}

TEST(FilterFunctionTest, BoxFilterTest)
{
  const ptrdiff_t h = 20;
  const ptrdiff_t w = 10;
  Image2d<int> src(h, w);
  fill(src, 1);

  Image2d<int> filt(h, w);
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
  Image2d<int> src(h, w);
  fill(src, 1);

  Image2d<int> filt(h, w);
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
  Image2d<int> src(h, w);
  fill(src, 1);

  Image2d<int> filt(h, w);
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
  Image2d<int> src(h, w);
  foreach2d(src, y, x)
  {
    src(y, x) = x < w / 2 ? 1 : 2;
  }

  Image2d<int> filt(h, w);
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
  Image2d<int> src(h, w);
  foreach2d(src, y, x)
  {
    src(y, x) = y < h / 2 ? 1 : 2;
  }

  Image2d<int> filt(h, w);
  box_filter_y(src, 1, BorderCondition::BC_WRAP, filt);
  
  // Check border pixels.
  for (ptrdiff_t j = 0; j < w; ++j)
  {
    ASSERT_EQ(filt(0, j), 4);
    ASSERT_EQ(filt(h - 1, j), 5);
  }
}

TEST(MorphologicalFunctionTest, TestErosion)
{
  // Small 5x5 image with 3x3 square in the center.
  Image2d<int> src(5, 5);
  fill(src, 0);
  for (int i = 1; i < 4; ++i)
    for (int j = 1; j < 4; ++j)
      src(i, j) = 1;

  Image2d<int> eroded(5, 5);
  erode(src, 1, eroded);
  
  ASSERT_EQ(eroded(2, 2), 1);

  // Set the middle pixels to 0, so we can use the loop below.
  eroded(2, 2) = 0;
  foreach2d(eroded, y, x)
    ASSERT_EQ(eroded(y, x), 0);
}

TEST(CannyEdgeDetectionTest, AngleBinTest)
{
  std::unordered_map<int8_t, double> bin_center_angles = {
    {0, 0.}, 
    {1, constants::quarter_pi}, 
    {2, constants::half_pi}, 
    {3, constants::half_pi + constants::quarter_pi}
  };

  std::vector<double> intra_bin_offsets =
  { -constants::eighth_pi + 1e-4, 0., constants::eighth_pi - 1e-4 };

  for (const auto& [bin, center_angle] : bin_center_angles)
  {
    for (auto offset : intra_bin_offsets)
    {
      const auto angle = center_angle + offset;
      const auto calculated_bin = detail::canny_get_angle_bin(angle);
      ASSERT_EQ(bin, calculated_bin);
    }
  }
}

namespace detail
{
  template<typename T>
  void draw_circle(ptrdiff_t c_y, ptrdiff_t c_x, ptrdiff_t r, Image2d<T>& dst)
  {
    foreach2d(dst, y, x)
    {
      const auto dist_sq = pow((y - c_y), 2) + pow((x - c_x), 2);
      dst(y, x) = dist_sq < pow(r, 2) ? T(1) : T(0);
    }
  }
}

TEST(Image2dBasicTest, ExportArray)
{
  const ptrdiff_t h = 300;
  const ptrdiff_t w = 300;
  Image2d<float> src(h, w);
  detail::draw_circle(h / 2, w / 2, 50, src);

  Image2d<float> gauss(h, w);
  gauss_filter(src, 1, 1, 1.f, 1.f, BorderCondition::BC_CLAMP, gauss);

  Image2d<float> grad_x(h, w);
  sobel_x(src, BorderCondition::BC_CLAMP, grad_x);

  Image2d<float> grad_y(h, w);
  sobel_y(src, BorderCondition::BC_CLAMP, grad_y);

  const auto lo_thr = 1.f;
  const auto hi_thr = 2.f;
  Image2d<unsigned char> canny(h, w);
  canny_edge_detection(src, lo_thr, hi_thr, canny);

  export_image("test_src.ppm", src);
  export_image("test_gauss.ppm", gauss);
  export_image("test_sobel_x.ppm", grad_x);
  export_image("test_sobel_y.ppm", grad_y);
  export_image("test_canny.ppm", canny);
}
