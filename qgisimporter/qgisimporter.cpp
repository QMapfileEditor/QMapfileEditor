#include "qgisimporter.h"

QGisImporter::QGisImporter(QString const &qgsPath):
  qgsPath(qgsPath), mapFile(0) {}

QGisImporter::~QGisImporter() {
  if (mapFile)
    delete mapFile;
}

MapfileParser * QGisImporter::importMapFile() {
  MapfileParser * mf = new MapfileParser();


  return mf;
}
