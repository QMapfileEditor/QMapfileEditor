#include "qgisimporter.h"

#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
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

  // TODO: setting default width / height ? (the information is not
  // available into QGis XML format)

  // gets the projection
  QDomNode destinationSrsNode = doc.elementsByTagName("destinationsrs").at(0);

  QString proj4Str = destinationSrsNode.firstChildElement("spatialrefsys").firstChildElement("proj4").text();
  mf->setMapProjection(proj4Str);

  // Layers
  QDomNodeList layersNodes = doc.elementsByTagName("maplayer");

  qDebug() << layersNodes.size() << " layers to parse";

  for (int i = 0 ; i < layersNodes.size(); ++i) {
    QString layerName = layersNodes.at(i).firstChildElement("layername").text();
    QString dataStr = layersNodes.at(i).firstChildElement("datasource").text();
    QString typeStr =  layersNodes.at(i).firstChildElement("provider").text();
    QString projStr =  layersNodes.at(i).firstChildElement("srs").firstChildElement("spatialrefsys").firstChildElement("proj4").text();
    qDebug() <<  layerName << dataStr << typeStr << projStr;
    mf->addLayer(layerName, dataStr, projStr);
  }



  f.close();

  return mf;
}
