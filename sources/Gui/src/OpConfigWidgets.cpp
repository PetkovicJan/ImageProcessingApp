#include <Gui/OpConfigWidgets.hpp>

ThresholdConfigWidget::ThresholdConfigWidget(QWidget* parent) : QWidget(parent)
{
  auto validator = new QDoubleValidator(0.0, 255.0, 2, this);
  validator->setNotation(QDoubleValidator::Notation::StandardNotation);

  auto threshold_label = new QLabel("Threshold:");
  auto threshold_edit = new QLineEdit();
  threshold_edit->setValidator(validator);

  auto true_val_label = new QLabel("True value:");
  auto true_val_edit = new QLineEdit();
  true_val_edit->setValidator(validator);

  auto false_val_label = new QLabel("False value:");
  auto false_val_edit = new QLineEdit();
  false_val_edit->setValidator(validator);

  auto grid_layout = new QGridLayout();
  this->setLayout(grid_layout);

  grid_layout->addWidget(threshold_label, 0, 0);
  grid_layout->addWidget(threshold_edit, 0, 1);

  grid_layout->addWidget(true_val_label, 1, 0);
  grid_layout->addWidget(true_val_edit, 1, 1);

  grid_layout->addWidget(false_val_label, 2, 0);
  grid_layout->addWidget(false_val_edit, 2, 1);

  QObject::connect(threshold_edit, &QLineEdit::editingFinished, [this, threshold_edit]() 
    {
      QLocale locale;
      this->config_.thresh = locale.toDouble(threshold_edit->text());
      emit this->configurationChanged(config_);
    });

  QObject::connect(true_val_edit, &QLineEdit::editingFinished, [this, true_val_edit]() 
    {
      QLocale locale;
      this->config_.true_val = locale.toDouble(true_val_edit->text());
      emit this->configurationChanged(config_);
    });

  QObject::connect(false_val_edit, &QLineEdit::editingFinished, [this, false_val_edit]() 
    {
      QLocale locale;
      this->config_.false_val = locale.toDouble(false_val_edit->text());
      emit this->configurationChanged(config_);
    });
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
      this->config_.sigma_x = sigma_x_edit->text().toDouble();
      emit this->configurationChanged(config_);
    });

  QObject::connect(sigma_y_edit, &QLineEdit::editingFinished, [this, sigma_y_edit]() 
    {
      this->config_.sigma_y = sigma_y_edit->text().toDouble();
      emit this->configurationChanged(config_);
    });
}

FormWidget::FormWidget()
{
  layout_ = new QGridLayout();
  this->setLayout(layout_);
}
