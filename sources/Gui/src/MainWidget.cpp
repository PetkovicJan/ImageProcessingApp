#include <Gui/MainWidget.hpp>

#include <QToolBar>
#include <QVBoxLayout>

#include <iostream>

MainWidget::MainWidget()
{
  this->setFixedSize(800, 600);

  // Create and configure widgets.
  auto toolbar = new QToolBar();
  toolbar->setMovable(false);
  toolbar->setAllowedAreas(Qt::TopToolBarArea);

  auto load_action = toolbar->addAction("Load");
  auto save_action = toolbar->addAction("Save");

  display_ = new QGraphicsView();
  display_->setScene(new QGraphicsScene(this));

  // Configure layout.
  auto main_layout = new QVBoxLayout();
  this->setLayout(main_layout);

  main_layout->addWidget(toolbar);
  main_layout->addWidget(display_);

  // Make connections.
  QObject::connect(load_action, &QAction::triggered, [this]()
    {
      emit this->loadClicked();
    });

  QObject::connect(save_action, &QAction::triggered, []()
    {
      std::cout << "Save clicked\n";
    });
}

void MainWidget::setImage(QPixmap img)
{
  display_->scene()->clear();
  display_->scene()->addPixmap(img);
  display_->fitInView(display_->sceneRect(), Qt::KeepAspectRatio);
}
