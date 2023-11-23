#pragma once

#include <Core/OpConfigDefines.hpp>

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QValidator>
#include <QGridLayout>
#include <QLocale>

#include <optional>
#include <type_traits>

namespace detail
{
  template<typename T>
  std::optional<T> getValHelper(QString const& str)
  {
    QLocale locale;
    bool ok = false;

    T val;
    if constexpr (std::is_integral_v<T>)
    {
      val = locale.toInt(str, &ok);
    }
    else if constexpr (std::is_same_v<T, float>)
    {
      val = locale.toFloat(str, &ok);
    }
    else if constexpr (std::is_same_v<T, double>)
    {
      val = locale.toDouble(str, &ok);
    }
    else
    {
      val = T();
    }

    if (ok)
      return val;
    else
      return std::nullopt;
  }

  template<typename T>
  std::optional<T> getValHelper(QVariant const& var)
  {
    bool ok = false;

    T val;
    if constexpr (std::is_same_v<T, GradConfig::GradType>)
    {
      val = static_cast<GradConfig::GradType>(var.toInt(&ok));
    }

    if (ok)
      return val;
    else
      return std::nullopt;
  }

  template<typename T>
  QVariant getVariantHelper(T const& val)
  {
    if constexpr (std::is_same_v<T, GradConfig::GradType>)
    {
      return QVariant(static_cast<int>(val));
    }
    else
    {
      return QVariant(val);
    }
  }
}

class FormWidget : public QWidget
{
  Q_OBJECT

public:
  FormWidget(bool init_vals, QWidget* parent = nullptr);

  template<typename ValueT>
  QLineEdit* addLineEdit(QString const& prompt, ValueT* val_ptr, QValidator* validator = nullptr)
  {
    auto label = new QLabel(prompt);
    auto edit = new QLineEdit();
    if (validator)
    {
      edit->setValidator(validator);
    }

    if (init_vals_)
    {
      edit->setText(QString::number(*val_ptr));
    }

    layout_->addWidget(label, current_row_, 0);
    layout_->addWidget(edit, current_row_, 1);
    ++current_row_;

    QObject::connect(edit, &QLineEdit::editingFinished, [this, edit, val_ptr]() 
      {
        if (const auto opt_val = detail::getValHelper<ValueT>(edit->text()))
        {
          *val_ptr = opt_val.value();

          emit this->valueChanged();
        }
      });

    return edit;
  }

  template<typename ValueT>
  QComboBox* addComboBox(
    QString const& prompt,std::vector<std::pair<QString, ValueT>> items, ValueT* val_ptr)
  {
    auto label = new QLabel(prompt);
    auto combo_box = new QComboBox();
    for (const auto& [str, val] : items)
    {
      combo_box->addItem(str, detail::getVariantHelper(val));
    }

    if (init_vals_)
    {
      const auto init_val = *val_ptr;
      std::optional<int> index;
      for (int i = 0; i < combo_box->count(); ++i)
      {
        const auto opt_val = detail::getValHelper<ValueT>(combo_box->itemData(i));
        if (opt_val.has_value() && init_val == opt_val.value())
        {
          index = i;
          break;
        }
      }

      if (index.has_value())
      {
        combo_box->setCurrentIndex(index.value());
      }
    }

    layout_->addWidget(label, current_row_, 0);
    layout_->addWidget(combo_box, current_row_, 1);
    ++current_row_;

    QObject::connect(combo_box, &QComboBox::currentIndexChanged,
      [this, combo_box, val_ptr](int new_index)
      {
        if (const auto opt_val = detail::getValHelper<ValueT>(combo_box->itemData(new_index)))
        {
          *val_ptr = opt_val.value();

          emit this->valueChanged();
        }

      });

    return combo_box;;
  }

signals:
  void valueChanged();

private:

  QGridLayout* layout_ = nullptr;
  int current_row_ = 0;
  bool init_vals_;
};

class OpConfigWidget : public QWidget
{
  Q_OBJECT

public:
  OpConfigWidget(QWidget* parent = nullptr);

signals:
  void configurationChanged(OpConfig const& config);
};

class ThresholdConfigWidget : public OpConfigWidget
{
  Q_OBJECT

public:
  ThresholdConfigWidget(QWidget* parent = nullptr);
  ThresholdConfigWidget(ThresholdConfig const& config, QWidget* parent = nullptr);

private:
  void initWidget(bool init_entries);

  ThresholdConfig config_;
};

class FilterConfigWidget : public OpConfigWidget
{
  Q_OBJECT

public:
  FilterConfigWidget(QWidget* parent = nullptr);
  FilterConfigWidget(FilterConfig const& config, QWidget* parent = nullptr);

private:
  void initWidget(bool init_entries);

  FilterConfig config_;
};

class GradConfigWidget : public OpConfigWidget
{
  Q_OBJECT

public:
  GradConfigWidget(QWidget* parent = nullptr);
  GradConfigWidget(GradConfig const& config, QWidget* parent = nullptr);

private:
  void initWidget(bool init_entries);

  GradConfig config_;
};
