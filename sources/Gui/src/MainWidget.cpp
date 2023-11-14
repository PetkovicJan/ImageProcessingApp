#include <Gui/MainWidget.hpp>

#include <QLabel>
#include <QToolBar>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>

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

  auto op_list_label = new QLabel("Operation List:");
  op_list_layout_ = new QVBoxLayout();

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
  ops_layout->addWidget(op_list_label);
  ops_layout->addLayout(op_list_layout_);
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

  QObject::connect(add_op_button, &QPushButton::clicked, [this]() 
    {
      if (current_op_ == QString("Threshold"))
      {

      }
      else if (current_op_ == QString("Filter"))
      {
        auto filter_label = new QLabel(current_op_);
        op_list_layout_->addWidget(filter_label);

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

using ThresholdConfigWidget = QPushButton;

void MainWidget::handleNewOpearation(QString const& new_op)
{
  // Store currently selected operation.
  current_op_ = new_op;

  detail::clear_layout(op_config_layout_);

  QWidget* op_config_widget = nullptr;
  if (new_op == QString("Threshold"))
  {
    op_config_widget = new ThresholdConfigWidget();
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

FilterConfigWidget::FilterConfigWidget(QWidget* parent) : QWidget(parent)
{
  auto radius_validator = new QIntValidator(1, 10, this);

  auto radius_x_label = new QLabel("Kernel radius (x):");
  auto radius_x_edit = new QLineEdit();
  radius_x_edit->setValidator(radius_validator);

  auto radius_y_label = new QLabel("Kernel radius (y):");
  auto radius_y_edit = new QLineEdit();
  radius_y_edit->setValidator(radius_validator);

  auto sigma_validator = new QDoubleValidator(0.1, 10., 2, this);

  auto sigma_x_label = new QLabel("Sigma (x):");
  auto sigma_x_edit = new QLineEdit();
  sigma_x_edit->setValidator(sigma_validator);

  auto sigma_y_label = new QLabel("Sigma (y):");
  auto sigma_y_edit = new QLineEdit();
  sigma_y_edit->setValidator(sigma_validator);

  auto grid_layout = new QGridLayout();
  this->setLayout(grid_layout);

  grid_layout->addWidget(radius_x_label, 0, 0);
  grid_layout->addWidget(radius_x_edit, 0, 1);

  grid_layout->addWidget(radius_y_label, 1, 0);
  grid_layout->addWidget(radius_y_edit, 1, 1);

  grid_layout->addWidget(sigma_x_label, 2, 0);
  grid_layout->addWidget(sigma_x_edit, 2, 1);

  grid_layout->addWidget(sigma_y_label, 3, 0);
  grid_layout->addWidget(sigma_y_edit, 3, 1);

  QObject::connect(radius_x_edit, &QLineEdit::editingFinished, [this, radius_x_edit]() 
    {
      this->config_.kernel_radius_x = radius_x_edit->text().toInt();
      emit this->configurationChanged(config_);
    });

  QObject::connect(radius_y_edit, &QLineEdit::editingFinished, [this, radius_y_edit]() 
    {
      this->config_.kernel_radius_y = radius_y_edit->text().toInt();
      emit this->configurationChanged(config_);
    });
  
  QObject::connect(sigma_x_edit, &QLineEdit::editingFinished, [this, sigma_x_edit]() 
    {
      this->config_.sigma_x = sigma_x_edit->text().toInt();
      emit this->configurationChanged(config_);
    });

  QObject::connect(sigma_y_edit, &QLineEdit::editingFinished, [this, sigma_y_edit]() 
    {
      this->config_.sigma_y = sigma_y_edit->text().toInt();
      emit this->configurationChanged(config_);
    });
}
