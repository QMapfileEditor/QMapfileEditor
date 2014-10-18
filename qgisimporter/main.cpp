#include <QApplication>
#include <QtCore/QDebug>
#include <QTemporaryFile>
#include <QTextStream>

#include "qgisimporter.h"

int main(int argc, char ** argv) {
   qDebug() << "QGis Importer test launched";

   QGisImporter *imp = NULL;

   QApplication(argc, argv);


   MapfileParser * p = NULL;
   if (argc > 1) {
     imp = new QGisImporter(argv[1]);
   } else {
     qDebug() << "Usage: " << argv[0] <<  "<QGis project>";
     return 0;
   }

   p = imp->importMapFile();

   // Touch'ing a temporary file
   //
   // Doing so, because the Mapserver API to save a mapfile into a char * is
   // fiddling with stdout, I don't feel it very secure in term of race
   // condition, if another thread ever pollutes the standard output during
   // dumping the memory object.
   //
   // See mapfile.c: char* msWriteMapToString(mapObj *map)
   //
   QTemporaryFile * f = new QTemporaryFile();
   f->open();
   f->close();
   qDebug() << "";

   p->saveMapfile(f->fileName());

   // reopens the file
   f->open();
   QTextStream in(f);
   qDebug() << in.readAll().toStdString().c_str();
   f->close();

   if (f)
     delete f;
   if (imp)
     delete imp;

  qDebug() << "QGis importer test about to end";
  return 0;
}
