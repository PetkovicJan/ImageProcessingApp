#pragma once

#include <QLabel>
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
  Q_OBJECT

public:
  ImageDisplayWidget(QWidget* parent = nullptr);

  void setImage(QImage img);

  void setHoveredPixelValue(int x, int y, float val);

signals:
  void imageHovered(QPointF const& pt);

private:
  ImageView* image_view_ = nullptr;

  QLabel* label_x_ = nullptr;
  QLabel* label_y_ = nullptr;
  QLabel* label_val_ = nullptr;
};