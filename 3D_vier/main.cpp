#include <locale.h>

#include <QApplication>
#include <QPainter>

#include "mainwindow.hpp"
int main(int argc, char *argv[]) {
  qDebug() << setlocale(LC_ALL, "en_US.UTF-8");
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}