/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: 
 * Author: Pierre Mauduit
 *
 **********************************************************************
 * Copyright (c) 2014, Pierre Mauduit
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of this Software or works derived from this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 ****************************************************************************/

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <stdlib.h>

#include <gdal.h>
#include <ogr_api.h>

#include "mainwindow.h"

extern "C" {
  extern int msDebugInitFromEnv();
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

#ifdef QT_DEBUG
  setenv("MS_ERRORFILE", "stdout", 1);
  setenv("MS_DEBUGLEVEL", "4", 1);
  msDebugInitFromEnv();
#endif // QT_DEBUG

  // loading translations / qtlinguist
  QTranslator trans;
  trans.load(QString(QLocale::system().name()), ":/lang/");
  a.installTranslator(& trans);

  // GDAL/OGR initialization
  GDALAllRegister();
  OGRRegisterAll();

  MainWindow w;

  // inits gdal / ogr drivers
  w.gdalGdalDrivers.clear();
  w.gdalOgrDrivers.clear();
  w.gdalGdalDrivers << "";
  w.gdalOgrDrivers << "";

  for (int i = 0; i < GDALGetDriverCount(); ++i) {
    GDALDriverH d = GDALGetDriver(i);
    w.gdalGdalDrivers << GDALGetDriverShortName(d) ;
  }
  for (int i = 0; i < OGRGetDriverCount(); ++i) {
    OGRSFDriverH d = OGRGetDriver(i);
    w.gdalOgrDrivers << OGR_Dr_GetName(d);
  }
  w.gdalOgrDrivers.removeDuplicates();
  w.gdalOgrDrivers.sort();
  w.gdalGdalDrivers.removeDuplicates();
  w.gdalGdalDrivers.sort();







  w.show();
  // preloads a mapfile if given as argument
  if (argc > 1) {
   w.openMapfile(argv[1]);
  }

  int ret = a.exec();

  // uninitialization
  for (int i = 0; i < GDALGetDriverCount(); i++) {
    GDALDriverH d = GDALGetDriver(i);
    GDALDeregisterDriver(d);
  }
  OGRCleanupAll();


  return ret;
}
