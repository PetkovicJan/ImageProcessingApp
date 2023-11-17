#pragma once

#include <QGraphicsView>

class ImageView : public QGraphicsView
{
  Q_OBJECT

public:
  ImageView(QWidget* parent = nullptr);

  void setImage(QPixmap img);
};