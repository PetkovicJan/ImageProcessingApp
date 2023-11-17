#include <Gui/ImageDisplayWidget.hpp>

ImageView::ImageView(QWidget* parent) : QGraphicsView(parent)
{
  this->setScene(new QGraphicsScene(this));
}

void ImageView::setImage(QPixmap img)
{
  this->scene()->clear();
  this->scene()->addPixmap(img);
  this->fitInView(this->sceneRect(), Qt::KeepAspectRatio);
}
