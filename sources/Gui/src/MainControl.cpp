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
        img(y, x) = qGray(rgb);
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
        const auto val = img(y, x);
        QColor color;
        color.setRgbF(val, val, val);
        row[x] = color.rgb();
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
}
