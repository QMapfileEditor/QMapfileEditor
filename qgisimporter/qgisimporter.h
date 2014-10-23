#ifndef QGISIMPORTER_H
#define QGISIMPORTER_H

#include <QString>

#include "../parser/mapfileparser.h"


class QGisImporter  : QObject {

 Q_OBJECT

 public:
  QGisImporter(QString const & qgsPath);
  ~QGisImporter();

  MapfileParser * importMapFile();

 private:
  QString qgsPath;
  MapfileParser * mapFile;

};


#endif // QGISIMPORTER_H

