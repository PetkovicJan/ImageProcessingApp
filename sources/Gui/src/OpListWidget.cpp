#include <Gui/OpListWidget.hpp>

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

void OpListWidget::addOperation(QString const& operation)
{
  QWidget* op_widget = nullptr;
  if (operation == QString("Threshold"))
  {
    op_widget = new QLabel(operation);
  }
  else if (operation == QString("Filter"))
  {
    op_widget = new QLabel(operation);
  }

  op_list_layout_->addWidget(op_widget);
}
