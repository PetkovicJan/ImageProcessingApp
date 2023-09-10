#pragma once

#include <Gui/MainWidget.hpp>

#include <Core/Core.hpp>

class MainControl
{
public:
  explicit MainControl(MainWidget* main_widget);

private:
  Image2d<float> current_img_;
};