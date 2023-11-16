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

  display_ = new QGraphicsView();
  display_->setScene(new QGraphicsScene(this));

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
  main_layout->addWidget(display_, 4);
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
      this->handleNewOpearation(new_op_name);
    });

  QObject::connect(add_op_button, &QPushButton::clicked, [this, op_list_widget]() 
    {
      op_list_widget->addOperation(current_op_);

      if (current_op_ == QString("Threshold"))
      {
        emit thresholdOpAdded(threshold_config_);

      }
      else if (current_op_ == QString("Filter"))
      {
        emit filterOpAdded(filter_config_);
      }
    });

  QObject::connect(execute_button, &QPushButton::clicked, [this]() 
    {
      emit this->executeClicked();
    });
}

void MainWidget::setImage(QPixmap img)
{
  display_->scene()->clear();
  display_->scene()->addPixmap(img);
  display_->fitInView(display_->sceneRect(), Qt::KeepAspectRatio);
}

void MainWidget::handleNewOpearation(QString const& new_op)
{
  // Store currently selected operation.
  current_op_ = new_op;

  detail::clear_layout(op_config_layout_);

  QWidget* op_config_widget = nullptr;
  if (new_op == QString("Threshold"))
  {
    auto threshold_config_widget = new ThresholdConfigWidget();
    QObject::connect(threshold_config_widget, &ThresholdConfigWidget::configurationChanged,
      [this](ThresholdConfig const& config) 
      {
        this->threshold_config_ = config;
      });
    op_config_widget = threshold_config_widget;
  }
  else if (new_op == QString("Filter"))
  {
    auto filter_config_widget = new FilterConfigWidget();
    QObject::connect(filter_config_widget, &FilterConfigWidget::configurationChanged,
      [this](FilterConfig const& config) 
      {
        this->filter_config_ = config;
      });
    op_config_widget = filter_config_widget;
  }
  else
  {
    throw std::runtime_error(std::string("Selected operation not supported: ") + new_op.toStdString());
  }

  op_config_layout_->addWidget(op_config_widget);
}
