#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  // loading translations / qtlinguist
  QTranslator trans;
  trans.load(QString(":/lang/" + QLocale::system().name()));
  a.installTranslator(& trans);

  MainWindow w;
  w.show();

  return a.exec();
}
