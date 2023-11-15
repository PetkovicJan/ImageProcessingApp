#pragma once

#include <Core/OpConfigDefines.hpp>

#include <QWidget.h>

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
