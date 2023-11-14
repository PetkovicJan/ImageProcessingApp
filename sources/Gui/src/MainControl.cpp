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

    foreach_y(img, y)
    {
      // QImage has a method pixel() to access each individual pixel value, 
      // but the documentation suggests the scanLine() method in case where 
      // we want to access all the pixels for performance reasons.
      const auto row = reinterpret_cast<QRgb*>(qimg.scanLine(y));
      foreach_x(img, x)
      {
        const auto val = int(img(y, x));
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

      main_widget->setImage(QPixmap::fromImage(loaded_img));
    });

    QObject::connect(main_widget, &MainWidget::thresholdOpAdded,
    [this](ThresholdConfig const& config)
    {
        this->op_chain_.addOperation(std::make_unique<ThresholdOp>(
          config.thresh,
          config.true_val,
          config.false_val));
    });
    
    QObject::connect(main_widget, &MainWidget::filterOpAdded,
    [this](FilterConfig const& config)
    {
      this->op_chain_.addOperation(std::make_unique<FilterOp>(
        config.kernel_radius_x,
        config.kernel_radius_y,
        config.sigma_x,
        config.sigma_y));
    });

  QObject::connect(main_widget, &MainWidget::executeClicked, 
    [this, main_widget]() 
    {
      Image2d<float> result(current_img_.height(), current_img_.width());
      this->op_chain_.executeChain(current_img_, result);

      const auto qimg = detail::create_qimage_from_image2d(result);
      main_widget->setImage(QPixmap::fromImage(qimg));
    });
}
