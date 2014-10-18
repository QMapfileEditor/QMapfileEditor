#ifndef QGISIMPORTER_H
#define QGISIMPORTER_H

#include <QString>

#include "../mapfileparser.h"


class QGisImporter {

 public:
  QGisImporter(QString const & qgsPath);
  ~QGisImporter();

  MapfileParser * importMapFile();

 private:
  QString qgsPath;
  MapfileParser * mapFile;

};


#endif // QGISIMPORTER_H

