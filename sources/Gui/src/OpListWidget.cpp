#include <Gui/OpListWidget.hpp>
#include <Gui/OpConfigWidgets.hpp>

#include <QLabel>
#include <QPushButton>
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

    std::pair<QString, OpConfigWidget*> operator()(GradConfig const& config)
    {
      return std::make_pair(QString("Gradient"), new GradConfigWidget(config));
    }
  };

  void remove_widget(QLayout* layout, QWidget* widget)
  {
    if (widget && layout)
    {
      layout->removeWidget(widget);
      delete widget;
    }
  }
}

OpListItem::OpListItem(OpConfig const& config, QWidget* parent) : QWidget(parent)
{
  const auto [name, op_config_widget] = std::visit(detail::OpConfigWidgetFactory{}, config);

  auto op_label = new QLabel(name);
  auto remove_button = new QPushButton("R");

  auto main_layout = new QVBoxLayout();
  this->setLayout(main_layout);

  auto top_layout = new QHBoxLayout();
  main_layout->addLayout(top_layout);
  main_layout->addWidget(op_config_widget);

  top_layout->addWidget(op_label);
  top_layout->addStretch();
  top_layout->addWidget(remove_button);

  QObject::connect(op_config_widget, &OpConfigWidget::configurationChanged,
    [this](OpConfig const& config)
    {
      emit configChanged(config);
    });

  QObject::connect(remove_button, &QPushButton::clicked,
    [this]()
    {
      emit removeClicked();
    });
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
  auto op_list_item = new OpListItem(config);
  const auto op_id = current_op_id_++;

  op_list_layout_->addWidget(op_list_item);

  QObject::connect(op_list_item, &OpListItem::configChanged,
    [this, op_id](OpConfig const& config) 
    {
      emit configChanged(op_id, config);
    });

  QObject::connect(op_list_item, &OpListItem::removeClicked,
    [this, op_list_item, op_id]() 
    {
      detail::remove_widget(op_list_layout_, op_list_item);

      emit opRemoved(op_id);
    });

  return op_id;
}
