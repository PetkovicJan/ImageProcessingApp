#pragma once

#include <Gui/MainWidget.hpp>

#include <Core/Core.hpp>

namespace detail
{
  void create_image2d_from_qimage(QImage const& qimg, Image2d<float>& img);
  QImage create_qimage_from_image2d(Image2d<float> const& img);
}

class MainControl
{
public:
  explicit MainControl(MainWidget* main_widget);

private:
  Image2d<float> current_img_;

  OperationChain op_chain_;
};