#include <gtest/gtest.h>

#include <Gui/MainControl.hpp>

namespace detail
{
  bool are_equal(Image2d<float> const& img1, Image2d<float> const& img2)
  {
    if (img1.height() != img2.height())
      return false;

    if (img1.width() != img2.width())
      return false;

    foreach2d(img1, y, x)
    {
      const auto val1 = img1(y, x);
      const auto val2 = img2(y, x);
      if (std::abs(val1 - val2) > 1e-2f)
      {
        return false;
      }
    }

    return true;
  }
}

TEST(ImageConversion, ConversionIsReversible)
{
  Image2d<float> src(20, 20);

  // Fill with something.
  fill(src, 0.f);
  for(int i = 3; i < 7; ++i)
    for (int j = 10; j < 13; ++j)
    {
      src(i, j) = i + j;
    }

  const auto qimg = detail::create_qimage_from_image2d(src);

  Image2d<float> dst;
  detail::create_image2d_from_qimage(qimg, dst);

  // Transforming Image2d to QImage and back to Image2d 
  // should render the same image.
  ASSERT_TRUE(detail::are_equal(src, dst));
}
