#include <Gui/MainWidget.hpp>
#include <Gui/MainControl.hpp>

#include <QApplication>
#include <QMainWindow>

int main(int argc, char** argv)
{
  QApplication a(argc, argv);

  auto main_widget = new MainWidget();
  MainControl main_control(main_widget);

  QMainWindow main_window;
  main_window.setCentralWidget(main_widget);
  main_window.move(200, 200);
  main_window.show();

  return a.exec();
}

