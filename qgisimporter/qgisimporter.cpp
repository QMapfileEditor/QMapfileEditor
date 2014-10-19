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

  qDebug() << "QGIS project XML loaded, parsing ...";

  MapfileParser * mf = new MapfileParser();

  // gets the title
  QDomNode titleNode = doc.elementsByTagName("title").at(0);
  mf->setMapName(titleNode.toElement().text());


  // gets the extent
  float xmin, ymin, xmax, ymax;
  QDomNode extent = doc.elementsByTagName("extent").at(0);

  xmin = extent.firstChildElement("xmin").text().toFloat();
  xmax = extent.firstChildElement("xmax").text().toFloat();
  ymin = extent.firstChildElement("ymin").text().toFloat();
  ymax = extent.firstChildElement("ymax").text().toFloat();

  mf->setMapExtent(xmin, ymin, xmax, ymax);

  // units
  QDomNode mapcanvasNode = doc.elementsByTagName("mapcanvas").at(0);
  QString units = mapcanvasNode.firstChildElement("units").text();
  // TODO: check possible values for units QGis-side
  mf->setMapUnits(units == "degrees" ? "dd" : units);


  f.close();

  return mf;
}
