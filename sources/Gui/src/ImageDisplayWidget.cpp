#include <Gui/ImageDisplayWidget.hpp>

#include <QLabel>
#include <QMouseEvent>
#include <QLayout>

ImageView::ImageView(QWidget* parent) : QGraphicsView(parent)
{
  this->setMouseTracking(true);
  this->setScene(new QGraphicsScene(this));
}

void ImageView::setImage(QPixmap img)
{
  this->scene()->clear();
  this->scene()->addPixmap(img);
  this->fitInView(this->sceneRect(), Qt::KeepAspectRatio);
}

void ImageView::mouseMoveEvent(QMouseEvent* event)
{
  const auto image_pos = this->mapToScene(event->pos());
  emit imageHovered(image_pos);
}

ImageDisplayWidget::ImageDisplayWidget(QWidget* parent)
{
  // Create widgets.
  auto label_x = new QLabel("x: 0");
  auto label_y = new QLabel("y: 0");
  auto label_val = new QLabel("value: 0");

  image_view_ = new ImageView();

  // Setup layout.
  auto main_layout = new QVBoxLayout();
  this->setLayout(main_layout);

  auto stats_layout = new QHBoxLayout();
  main_layout->addLayout(stats_layout);
  main_layout->addWidget(image_view_);

  stats_layout->addWidget(label_x);
  stats_layout->addWidget(label_y);
  stats_layout->addWidget(label_val);
  stats_layout->addStretch();

  // Make connections.
  QObject::connect(image_view_, &ImageView::imageHovered, 
    [this, label_x, label_y, label_val](QPointF const& img_pos) 
    {
      const auto pt = img_pos.toPoint();
      const auto img_sz = displayed_img_.size();
      if (
        pt.x() >= 0 && pt.x() < img_sz.width() && 
        pt.y() >= 0 && pt.y() < img_sz.height())
      {
        const auto rgb = displayed_img_.pixel(pt);
        const auto gray_val = 0.333333f * float(qRed(rgb) + qGreen(rgb) + qBlue(rgb));
        label_x->setText(QString("x: %1").arg(pt.x()));
        label_y->setText(QString("y: %1").arg(pt.y()));
        label_val->setText(QString("value: %1").arg(gray_val));
      }
    });
}

void ImageDisplayWidget::setImage(QImage img)
{
  displayed_img_ = img;
  image_view_->setImage(QPixmap::fromImage(img));
}
