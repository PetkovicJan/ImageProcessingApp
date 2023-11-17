#include <Gui/OpListWidget.hpp>
#include <Gui/OpConfigWidgets.hpp>

#include <QLabel>
#include <QBoxLayout>

OpListWidget::OpListWidget(QWidget* parent) : QWidget(parent)
{
  auto op_list_label = new QLabel("Operation List:");
  op_list_layout_ = new QVBoxLayout();

  auto main_layout = new QVBoxLayout();
  main_layout->addWidget(op_list_label);
  main_layout->addLayout(op_list_layout_);

  this->setLayout(main_layout);
}

int OpListWidget::addOperation(QString const& operation)
{
  QWidget* op_label = new QLabel(operation);

  OpConfigWidget* op_config_widget = nullptr;
  if (operation == QString("Threshold"))
  {
    op_config_widget = new ThresholdConfigWidget();
  }
  else if (operation == QString("Filter"))
  {
    op_config_widget = new FilterConfigWidget();
  }

  QObject::connect(op_config_widget, &OpConfigWidget::configurationChanged,
    [this, op_id = current_op_id_](OpConfig const& config) 
    {
      emit configChanged(op_id, config);
    });

  op_list_layout_->addWidget(op_label);
  op_list_layout_->addWidget(op_config_widget);

  return current_op_id_++;
}
