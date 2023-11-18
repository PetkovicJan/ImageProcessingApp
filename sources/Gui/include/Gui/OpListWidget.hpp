#pragma once

#include <Core/OpConfigDefines.hpp>

#include <QWidget>
#include <QLayout>

class OpListItem : public QWidget
{
  Q_OBJECT

public:
  OpListItem(OpConfig const& config, QWidget* parent = nullptr);

signals:
  void configChanged(OpConfig const& config);
  void removeClicked();
};

class OpListWidget : public QWidget
{
  Q_OBJECT

public:
  OpListWidget(QWidget* parent = nullptr);

  // Adds operation to the list and returns its unique ID.
  int addOperation(OpConfig const& config);

signals:
  void configChanged(int op_id, OpConfig const& config);
  void opRemoved(int op_id);

private:
  QLayout* op_list_layout_ = nullptr;

  int current_op_id_ = 0;
};