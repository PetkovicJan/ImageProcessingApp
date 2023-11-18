#include <Gui/OpConfigWidgets.hpp>

FormWidget::FormWidget(bool init_vals, QWidget* parent) : 
  QWidget(parent), init_vals_(init_vals)
{
  layout_ = new QGridLayout();
  this->setLayout(layout_);
}

OpConfigWidget::OpConfigWidget(QWidget* parent) : QWidget(parent) {}

ThresholdConfigWidget::ThresholdConfigWidget(QWidget* parent) : OpConfigWidget(parent)
{
  initWidget(false);
}

ThresholdConfigWidget::ThresholdConfigWidget(ThresholdConfig const& config, QWidget* parent) : 
  OpConfigWidget(parent), config_(config)
{
  initWidget(true);
}

void ThresholdConfigWidget::initWidget(bool init_entries)
{
  auto validator = new QDoubleValidator(0.0, 255.0, 2, this);
  validator->setNotation(QDoubleValidator::Notation::StandardNotation);

  auto threshold_form_widget = new FormWidget(init_entries);
  threshold_form_widget->addEntry<float>("Threshold:", &config_.thresh, validator);
  threshold_form_widget->addEntry<float>("True value:", &config_.true_val, validator);
  threshold_form_widget->addEntry<float>("False value:", &config_.false_val, validator);

  auto layout = new QVBoxLayout();
  layout->addWidget(threshold_form_widget);

  this->setLayout(layout);

  QObject::connect(threshold_form_widget, &FormWidget::entryChanged, [this]() 
    {
      emit this->configurationChanged(config_);
    });
}

FilterConfigWidget::FilterConfigWidget(QWidget* parent) : OpConfigWidget(parent)
{
  initWidget(false);
}

FilterConfigWidget::FilterConfigWidget(FilterConfig const& config, QWidget* parent) :
  OpConfigWidget(parent), config_(config)
{
  initWidget(true);
}

void FilterConfigWidget::initWidget(bool init_entries)
{
  auto radius_validator = new QIntValidator(1, 10, this);
  auto sigma_validator = new QDoubleValidator(0.1, 10., 2, this);

  auto filter_form_widget = new FormWidget(init_entries);
  filter_form_widget->addEntry<ptrdiff_t>("Kernel radius (x):", &config_.kernel_radius_x, radius_validator);
  filter_form_widget->addEntry<ptrdiff_t>("Kernel radius (y):", &config_.kernel_radius_y, radius_validator);
  filter_form_widget->addEntry<float>("Sigma (x):", &config_.sigma_x, sigma_validator);
  filter_form_widget->addEntry<float>("Sigma (y):", &config_.sigma_y, sigma_validator);

  auto layout = new QVBoxLayout();
  layout->addWidget(filter_form_widget);

  this->setLayout(layout);

  QObject::connect(filter_form_widget, &FormWidget::entryChanged, [this]() 
    {
      emit this->configurationChanged(config_);
    });
}
