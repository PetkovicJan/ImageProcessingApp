#pragma once

#include <Core/OpConfigDefines.hpp>

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QValidator>
#include <QGridLayout>
#include <QLocale>

#include <optional>
#include <type_traits>

class FormWidget : public QWidget
{
  Q_OBJECT

public:
  FormWidget(QWidget* parent = nullptr);

  template<typename EntryT>
  void addEntry(QString const& prompt, EntryT* val_ptr, QValidator* validator = nullptr)
  {
    auto entry_label = new QLabel(prompt);
    auto entry_edit = new QLineEdit();
    if (validator)
    {
      entry_edit->setValidator(validator);
    }

    layout_->addWidget(entry_label, current_entry_idx_, 0);
    layout_->addWidget(entry_edit, current_entry_idx_, 1);
    ++current_entry_idx_;

    QObject::connect(entry_edit, &QLineEdit::editingFinished, [this, entry_edit, val_ptr]() 
      {
        if (const auto opt_val = get_val_helper<EntryT>(entry_edit->text()))
        {
          *val_ptr = opt_val.value();

          emit this->entryChanged();
        }
      });
  }

signals:
  void entryChanged();

private:
  template<typename T>
  std::optional<T> get_val_helper(QString const& str)
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

  QGridLayout* layout_ = nullptr;
  int current_entry_idx_ = 0;
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

private:
  ThresholdConfig config_;
};

class FilterConfigWidget : public OpConfigWidget
{
  Q_OBJECT

public:
  FilterConfigWidget(QWidget* parent = nullptr);

private:
  FilterConfig config_;
};
