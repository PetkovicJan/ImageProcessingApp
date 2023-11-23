#include <Gui/MainControl.hpp>

#include <QFileDialog>

#include <iostream>

namespace detail
{
  void create_image2d_from_qimage(QImage const& qimg, Image2d<float>& img)
  {
    img.alloc(qimg.height(), qimg.width());

    foreach_y(img, y)
    {
      // QImage has a method pixel() to access each individual pixel value, 
      // but the documentation suggests the scanLine() method in case where 
      // we want to access all the pixels for performance reasons.
      const auto row = reinterpret_cast<QRgb const*>(qimg.constScanLine(y));
      foreach_x(img, x)
      {
        const auto& rgb = row[x];
        img(y, x) = 0.333333f * float(qRed(rgb) + qGreen(rgb) + qBlue(rgb));
      }
    }
  }

  QImage create_qimage_from_image2d(Image2d<float> const& img)
  {
    QImage qimg(img.width(), img.height(), QImage::Format_RGB32);

    const auto max_val = max_value(img);
    const auto min_val = min_value(img);
    const auto scale = 254.f / (max_val - min_val);
    foreach_y(img, y)
    {
      // QImage has a method pixel() to access each individual pixel value, 
      // but the documentation suggests the scanLine() method in case where 
      // we want to access all the pixels for performance reasons.
      const auto row = reinterpret_cast<QRgb*>(qimg.scanLine(y));
      foreach_x(img, x)
      {
        const auto val = int(scale * (img(y, x) - min_val));
        row[x] = qRgb(val, val, val);
      }
    }

    return qimg;
  }
}

MainControl::MainControl(MainWidget* main_widget)
{
  QObject::connect(main_widget, &MainWidget::loadClicked, [this, main_widget]()
    {
      const auto image_name = QFileDialog::getOpenFileName(
        main_widget, "Load Image", "/home", "Images (*.png *.jpg *.ppm)");

      if (image_name.isNull()) return;

      QImage loaded_img;
      loaded_img.load(image_name);

      detail::create_image2d_from_qimage(loaded_img, current_img_);

      this->setDisplayedImage(main_widget, current_img_);
    });

  QObject::connect(main_widget, &MainWidget::imageHovered, [this, main_widget](QPointF const& img_pos)
    {
      if (!displayed_img_) return;

      const auto pos = Position(img_pos.y(), img_pos.x());
      if (displayed_img_->isValid(pos))
      {
        main_widget->setHoveredPixelValue(pos.x, pos.y, (*displayed_img_)(pos));
      }
    });

  QObject::connect(main_widget, &MainWidget::opAdded,
    [this](int op_id, OpConfig const& config)
    {
      this->op_chain_.addOperation(op_id, config);
    });
    
  QObject::connect(main_widget, &MainWidget::opChanged,
    [this](int op_id, OpConfig const& config)
    {
      this->op_chain_.modifyOperation(op_id, config);
    });

  QObject::connect(main_widget, &MainWidget::opRemoved,
    [this](int op_id)
    {
      this->op_chain_.removeOperation(op_id);
    });

  QObject::connect(main_widget, &MainWidget::executeClicked, 
    [this, main_widget]() 
    {
      result_img_.alloc(current_img_.height(), current_img_.width());
      this->op_chain_.executeChain(current_img_, result_img_);

      this->setDisplayedImage(main_widget, result_img_);
    });
}

void MainControl::setDisplayedImage(MainWidget* widget, Image2d<float> const& img)
{
  displayed_img_ = &img;

  const auto qimg = detail::create_qimage_from_image2d(img);
  widget->setImage(qimg);
}
