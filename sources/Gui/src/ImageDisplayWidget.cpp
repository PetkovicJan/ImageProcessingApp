#include <Gui/ImageDisplayWidget.hpp>

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
  label_x_ = new QLabel("x: 0");
  label_y_ = new QLabel("y: 0");
  label_val_ = new QLabel("value: 0");

  image_view_ = new ImageView();

  // Setup layout.
  auto main_layout = new QVBoxLayout();
  this->setLayout(main_layout);

  auto stats_layout = new QHBoxLayout();
  main_layout->addLayout(stats_layout);
  main_layout->addWidget(image_view_);

  stats_layout->addWidget(label_x_);
  stats_layout->addWidget(label_y_);
  stats_layout->addWidget(label_val_);
  stats_layout->addStretch();

  // Make connections.
  QObject::connect(image_view_, &ImageView::imageHovered,
    [this](QPointF const& img_pos)
    {
      emit this->imageHovered(img_pos);
    });
}

void ImageDisplayWidget::setImage(QImage img)
{
  image_view_->setImage(QPixmap::fromImage(img));
}

void ImageDisplayWidget::setHoveredPixelValue(int x, int y, float val)
{
  label_x_->setText(QString("x: %1").arg(x));
  label_y_->setText(QString("y: %1").arg(y));
  label_val_->setText(QString("value: %1").arg(val));
}
