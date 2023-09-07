#pragma once

#include <QWidget>
#include <QGraphicsView>

class MainWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MainWidget();

  void setImage(QPixmap img);

signals:
  void loadClicked();

private:
  QGraphicsView* display_ = nullptr;
};
