#pragma once

#include <Gui/ImageDisplayWidget.hpp>

#include <Core/OpConfigDefines.hpp>

#include <QWidget>
#include <QGraphicsView>
#include <QVBoxLayout>

class MainWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MainWidget();

  void setImage(QImage img);

  void setHoveredPixelValue(int x, int y, float val);

signals:
  void loadClicked();

  void imageHovered(QPointF const& pt);

  void opAdded(int op_id, OpConfig const& config);
  void opChanged(int op_id, OpConfig const& config);
  void opRemoved(int op_id);

  void executeClicked();

private:
  void onOperationSelected(QString const& new_op);

  ImageDisplayWidget* image_display_widget_ = nullptr;
  QVBoxLayout* op_config_layout_ = nullptr;

  // Currently active operation and corresponding configuration.
  OpConfig current_op_config_;
};
