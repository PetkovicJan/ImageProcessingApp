#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QVBoxLayout>

struct FilterConfig
{
  ptrdiff_t kernel_radius_x;
  ptrdiff_t kernel_radius_y;

  float sigma_x;
  float sigma_y;
};

class FilterConfigWidget : public QWidget
{
  Q_OBJECT

public:
  FilterConfigWidget(QWidget* parent = nullptr);

signals:
  void configurationChanged(FilterConfig const& config);

private:
  FilterConfig config_;
};

class MainWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MainWidget();

  void setImage(QPixmap img);

signals:
  void loadClicked();
  void filterOpAdded(FilterConfig const& config);
  void executeClicked();

private:
  void handleNewOpearation(QString const& new_op);

  QGraphicsView* display_ = nullptr;
  QVBoxLayout* op_config_layout_ = nullptr;
  QVBoxLayout* op_list_layout_ = nullptr;

  QString current_op_;
  FilterConfig filter_config_;
};
