#include <Gui/MainControl.hpp>

#include <QFileDialog>

namespace detail
{
  void create_array2d_from_image(QImage const& img, Image2d<float>& arr)
  {

  }
}

MainControl::MainControl(MainWidget* main_widget)
{
  QObject::connect(main_widget, &MainWidget::loadClicked, [this, main_widget]()
    {
      const auto image_name = QFileDialog::getOpenFileName(
        main_widget, "Load Image", "/home", "Images (*.png *.jpg)");

      QImage img;
      img.load(image_name);

      detail::create_array2d_from_image(img, current_img_);

      main_widget->setImage(QPixmap::fromImage(img));
    });
}
