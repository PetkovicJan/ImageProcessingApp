#pragma once

#include <Core/OpConfigDefines.hpp>

#include <QWidget>
#include <QGraphicsView>
#include <QVBoxLayout>

class ThresholdConfigWidget : public QWidget
{
  Q_OBJECT

public:
  ThresholdConfigWidget(QWidget* parent = nullptr);

signals:
  void configurationChanged(ThresholdConfig const& config);

private:
  ThresholdConfig config_;
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

  void thresholdOpAdded(ThresholdConfig const& config);
  void filterOpAdded(FilterConfig const& config);

  void executeClicked();

private:
  void handleNewOpearation(QString const& new_op);

  QGraphicsView* display_ = nullptr;
  QVBoxLayout* op_config_layout_ = nullptr;
  QVBoxLayout* op_list_layout_ = nullptr;

  QString current_op_;

  ThresholdConfig threshold_config_;
  FilterConfig filter_config_;
};
