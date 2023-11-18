#include <Gui/OpListWidget.hpp>
#include <Gui/OpConfigWidgets.hpp>

#include <QLabel>
#include <QBoxLayout>

namespace detail
{
  struct OpConfigWidgetFactory
  {
    std::pair<QString, OpConfigWidget*> operator()(ThresholdConfig const& config)
    {
      return std::make_pair(QString("Threshold"), new ThresholdConfigWidget(config));
    }

    std::pair<QString, OpConfigWidget*> operator()(FilterConfig const& config)
    {
      return std::make_pair(QString("Filter"), new FilterConfigWidget(config));
    }
  };
}

OpListWidget::OpListWidget(QWidget* parent) : QWidget(parent)
{
  auto op_list_label = new QLabel("Operation List:");
  op_list_layout_ = new QVBoxLayout();

  auto main_layout = new QVBoxLayout();
  main_layout->addWidget(op_list_label);
  main_layout->addLayout(op_list_layout_);

  this->setLayout(main_layout);
}

int OpListWidget::addOperation(OpConfig const& config)
{
  const auto [name, op_config_widget] = std::visit(detail::OpConfigWidgetFactory{}, config);

  QWidget* op_label = new QLabel(name);

  QObject::connect(op_config_widget, &OpConfigWidget::configurationChanged,
    [this, op_id = current_op_id_](OpConfig const& config) 
    {
      emit configChanged(op_id, config);
    });

  op_list_layout_->addWidget(op_label);
  op_list_layout_->addWidget(op_config_widget);

  return current_op_id_++;
}
