#include <Gui/OpConfigWidgets.hpp>

ThresholdConfigWidget::ThresholdConfigWidget(QWidget* parent) : QWidget(parent)
{
  // auto validator = new QDoubleValidator(0.0, 255.0, 2, this);
  // validator->setNotation(QDoubleValidator::Notation::StandardNotation);

  auto threshold_form_widget = new FormWidget();
  threshold_form_widget->addEntry<float>("Threshold:", &config_.thresh);
  threshold_form_widget->addEntry<float>("True value:", &config_.true_val);
  threshold_form_widget->addEntry<float>("False value:", &config_.false_val);

  auto layout = new QVBoxLayout();
  layout->addWidget(threshold_form_widget);

  this->setLayout(layout);

  QObject::connect(threshold_form_widget, &FormWidget::entryChanged, [this]() 
    {
      emit this->configurationChanged(config_);
    });
}

FilterConfigWidget::FilterConfigWidget(QWidget* parent) : QWidget(parent)
{
  // auto radius_validator = new QIntValidator(1, 10, this);
  // auto sigma_validator = new QDoubleValidator(0.1, 10., 2, this);

  auto filter_form_widget = new FormWidget();
  filter_form_widget->addEntry<ptrdiff_t>("Kernel radius (x):", &config_.kernel_radius_x);
  filter_form_widget->addEntry<ptrdiff_t>("Kernel radius (y):", &config_.kernel_radius_y);
  filter_form_widget->addEntry<float>("Sigma (x):", &config_.sigma_x);
  filter_form_widget->addEntry<float>("Sigma (y):", &config_.sigma_y);

  auto layout = new QVBoxLayout();
  layout->addWidget(filter_form_widget);

  this->setLayout(layout);

  QObject::connect(filter_form_widget, &FormWidget::entryChanged, [this]() 
    {
      emit this->configurationChanged(config_);
    });
}

FormWidget::FormWidget(QWidget* parent) : QWidget(parent)
{
  layout_ = new QGridLayout();
  this->setLayout(layout_);
}
