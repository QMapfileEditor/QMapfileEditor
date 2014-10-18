#include "qgisimporter.h"

#include <QDomDocument>
#include <QDomNode>
#include <QFile>

// temporary
#include <QtCore/QDebug>

QGisImporter::QGisImporter(QString const &qgsPath):
  qgsPath(qgsPath), mapFile(0) {}

QGisImporter::~QGisImporter() {
  if (mapFile)
    delete mapFile;
}

MapfileParser * QGisImporter::importMapFile() {

  QFile f(qgsPath);

  // some first basic checks on the filepath
  if (!f.open(QIODevice::ReadOnly)) {
    return NULL;
  }

  QDomDocument doc("QGisImporterXmlDoc");
  if (!doc.setContent(&f)) {
    f.close();
    return NULL;
  }

  qDebug() << "QGis project XML loaded, parsing ...";

  MapfileParser * mf = new MapfileParser();
  // gets the extent
  float xmin, ymin, xmax, ymax;
  QDomNode extent = doc.elementsByTagName("extent").at(0);
  // takes the first extent node
  qDebug() << extent.firstChildElement("xmin").text();
  qDebug() << extent.firstChildElement("xmax").text();
  qDebug() << extent.firstChildElement("ymin").text();
  qDebug() << extent.firstChildElement("ymax").text();

  f.close();

  return mf;
}
