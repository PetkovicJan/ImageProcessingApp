#include <Gui/MainGui.hpp>

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QFileDialog>

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

  auto display = new QGraphicsView();
  display->setScene(new QGraphicsScene(this));

  // Configure layout.
  auto main_layout = new QVBoxLayout();
  this->setLayout(main_layout);

  main_layout->addWidget(toolbar);
  main_layout->addWidget(display);

  // Make connections.
  QObject::connect(load_action, &QAction::triggered, [this, display] 
    {
      const auto image_name = QFileDialog::getOpenFileName(
        this, tr("Load Image"), "/home", tr("Images (*.png *.jpg)"));

      QPixmap img;
      img.load(image_name);

      display->scene()->addPixmap(img);
      display->fitInView(display->sceneRect(), Qt::KeepAspectRatio);
    });

  QObject::connect(save_action, &QAction::triggered, [] 
    {
      std::cout << "Save clicked\n";
    });
}

int main(int argc, char** argv)
{
  QApplication a(argc, argv);

  QMainWindow main_window;
  main_window.setCentralWidget(new MainWidget());
  main_window.move(200, 200);
  main_window.show();

  return a.exec();
}