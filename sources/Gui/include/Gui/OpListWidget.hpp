#pragma once

#include <QWidget>
#include <QLayout>

class OpListWidget : public QWidget
{
  Q_OBJECT

public:
  OpListWidget(QWidget* parent = nullptr);

  void addOperation(QString const& operation);

private:
  QLayout* op_list_layout_ = nullptr;
};