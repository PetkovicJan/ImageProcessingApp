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
  threshold_form_widget->addLineEdit<float>("Threshold:", &config_.thresh, validator);
  threshold_form_widget->addLineEdit<float>("True value:", &config_.true_val, validator);
  threshold_form_widget->addLineEdit<float>("False value:", &config_.false_val, validator);

  auto layout = new QVBoxLayout();
  layout->addWidget(threshold_form_widget);

  this->setLayout(layout);

  QObject::connect(threshold_form_widget, &FormWidget::valueChanged, [this]() 
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
  filter_form_widget->addLineEdit<ptrdiff_t>("Kernel radius (x):", &config_.kernel_radius_x, radius_validator);
  filter_form_widget->addLineEdit<ptrdiff_t>("Kernel radius (y):", &config_.kernel_radius_y, radius_validator);
  filter_form_widget->addLineEdit<float>("Sigma (x):", &config_.sigma_x, sigma_validator);
  filter_form_widget->addLineEdit<float>("Sigma (y):", &config_.sigma_y, sigma_validator);

  auto layout = new QVBoxLayout();
  layout->addWidget(filter_form_widget);

  this->setLayout(layout);

  QObject::connect(filter_form_widget, &FormWidget::valueChanged, [this]() 
    {
      emit this->configurationChanged(config_);
    });
}

GradConfigWidget::GradConfigWidget(QWidget* parent) : OpConfigWidget(parent)
{
  initWidget(false);
}

GradConfigWidget::GradConfigWidget(GradConfig const& config, QWidget* parent) : 
  OpConfigWidget(parent), config_(config)
{
  initWidget(true);
}

void GradConfigWidget::initWidget(bool init_entries)
{
  using GradT = GradConfig::GradType;

  const std::vector<std::pair<QString, GradT>> name_type_pairs = {
    {"Grad-X", GradT::GradX},
    {"Grad-Y", GradT::GradY},
    {"Grad-Abs", GradT::GradAbs} };

  auto grad_form_widget = new FormWidget(init_entries);
  grad_form_widget->addComboBox<GradT>("Type:", name_type_pairs, &config_.type);

  auto layout = new QVBoxLayout();
  layout->addWidget(grad_form_widget);

  this->setLayout(layout);

  QObject::connect(grad_form_widget, &FormWidget::valueChanged, [this]() 
    {
      emit this->configurationChanged(config_);
    });
}
