#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <gdal.h>
#include <ogr_api.h>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);



  // loading translations / qtlinguist
  QTranslator trans;
  trans.load(QString(QLocale::system().name()), ":/lang/");
  a.installTranslator(& trans);

  // GDAL/OGR initialization
  GDALAllRegister();
  OGRRegisterAll();

  MainWindow w;
  w.show();

  int ret = a.exec();

  // uninitialization
  for (int i = 0; i < GDALGetDriverCount(); i++) {
    GDALDriverH d = GDALGetDriver(i);
    GDALDeregisterDriver(d);
  }
  OGRCleanupAll();


  return ret;
}
