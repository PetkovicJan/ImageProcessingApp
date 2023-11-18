#include <Gui/MainWidget.hpp>
#include <Gui/OpConfigWidgets.hpp>
#include <Gui/OpListWidget.hpp>

#include <QLabel>
#include <QToolBar>
#include <QPushButton>
#include <QComboBox>

#include <iostream>
#include <vector>

namespace detail
{
  void clear_layout(QLayout* layout)
  {
    // Copied directly from the documentation.
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr)
    {
      delete child->widget();
      delete child;
    }
  }
}

MainWidget::MainWidget()
{
  this->setFixedSize(1000, 800);

  // Create and configure widgets.
  auto toolbar = new QToolBar();
  toolbar->setMovable(false);
  toolbar->setAllowedAreas(Qt::TopToolBarArea);

  auto load_action = toolbar->addAction("Load");
  auto save_action = toolbar->addAction("Save");

  image_display_widget_ = new ImageDisplayWidget();

  auto configure_op_label = new QLabel("Configure Operation");
  auto select_op_combo = new QComboBox();
  auto add_op_button = new QPushButton("Add Operation");
  auto execute_button = new QPushButton("Execute Operation");

  const std::vector<QString> op_names = { "Threshold", "Filter" };
  for (const auto& name : op_names)
  {
    select_op_combo->addItem(name);
  }

  op_config_layout_ = new QVBoxLayout();

  auto op_list_widget = new OpListWidget();

  // Configure layout.
  auto full_layout = new QVBoxLayout();
  this->setLayout(full_layout);

  auto main_layout = new QHBoxLayout();

  full_layout->addWidget(toolbar);
  full_layout->addLayout(main_layout);

  auto ops_layout = new QVBoxLayout();
  main_layout->addWidget(image_display_widget_, 4);
  main_layout->addLayout(ops_layout, 1);

  ops_layout->addWidget(configure_op_label);
  ops_layout->addWidget(select_op_combo);
  ops_layout->addLayout(op_config_layout_);
  ops_layout->addWidget(add_op_button);
  ops_layout->addWidget(op_list_widget);
  ops_layout->addWidget(execute_button);
  ops_layout->addStretch();

  // Make connections.
  QObject::connect(load_action, &QAction::triggered, [this]()
    {
      emit this->loadClicked();
    });

  QObject::connect(save_action, &QAction::triggered, []()
    {
      std::cout << "Save clicked\n";
    });

  QObject::connect(select_op_combo, &QComboBox::currentTextChanged, [this](QString const& new_op_name) 
    {
      this->onOperationSelected(new_op_name);
    });

  QObject::connect(add_op_button, &QPushButton::clicked, [this, op_list_widget]() 
    {
      const auto op_id = op_list_widget->addOperation(current_op_config_);

      emit opAdded(op_id, current_op_config_);
    });

  QObject::connect(op_list_widget, &OpListWidget::configChanged,
    [this](int op_id, OpConfig const& config)
    {
      emit opChanged(op_id, config);
    });

  QObject::connect(op_list_widget, &OpListWidget::opRemoved,
    [this](int op_id)
    {
      emit opRemoved(op_id);
    });

  QObject::connect(execute_button, &QPushButton::clicked, [this]()
    {
      emit this->executeClicked();
    });
}

void MainWidget::setImage(QImage img)
{
  image_display_widget_->setImage(img);
}

void MainWidget::onOperationSelected(QString const& new_op)
{
  OpConfigWidget* op_config_widget = nullptr;
  if (new_op == QString("Threshold"))
  {
    op_config_widget = new ThresholdConfigWidget();
  }
  else if (new_op == QString("Filter"))
  {
    op_config_widget = new FilterConfigWidget();
  }
  else
  {
    throw std::runtime_error(std::string("Selected operation not supported: ") + new_op.toStdString());
  }

  QObject::connect(op_config_widget, &OpConfigWidget::configurationChanged,
    [this](OpConfig const& config)
    {
      this->current_op_config_ = config;
    });

  detail::clear_layout(op_config_layout_);
  op_config_layout_->addWidget(op_config_widget);
}
