#pragma once

#include <QWidget>
#include <QGraphicsView>

class ImageView : public QGraphicsView
{
  Q_OBJECT

public:
  ImageView(QWidget* parent = nullptr);

  void setImage(QPixmap img);

signals:
  void imageHovered(QPointF const& img_pos);

protected:
  void mouseMoveEvent(QMouseEvent* event) override;
};

class ImageDisplayWidget : public QWidget
{
public:
  ImageDisplayWidget(QWidget* parent = nullptr);

  void setImage(QImage img);

private:
  ImageView* image_view_ = nullptr;

  // Keep current image in memory for fast pixel access.
  QImage displayed_img_;
};