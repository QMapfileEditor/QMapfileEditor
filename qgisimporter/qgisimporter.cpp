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

#include "mapserver.h"


#include "cpl_progress.h"
#include "cpl_minixml.h"
#include "ogr_core.h"


#include "qgisimporter.h"

// temporary
#include <QtCore/QDebug>

QGisImporter::QGisImporter(QString const &qgsPath):
  qgsPath(qgsPath), mapFile(0) {}

QGisImporter::~QGisImporter() {
  if (mapFile)
    delete mapFile;
}

int QGisImporter::getGeometryType(QString const & path) {
  int ret = -1;

  // Well, this code is legacy, but fsck it :-P
  // Should not be compatible GDAL >= 2.0

  // TODO: Anyway, Mapserver code should provide similar mechanisms,
  // I'm probably re-inventing the wheel here.

  OGRDataSourceH  hDS;
  hDS = OGROpen(path.toStdString().c_str(), 0, NULL);
  if(hDS == NULL)
  {
    return -1;
  }

  int layerCount = OGR_DS_GetLayerCount(hDS);

  if (layerCount <= 0) {
    OGRReleaseDataSource( hDS );
    return -1;
  }

  OGRLayerH layer =  OGR_DS_GetLayer (hDS, 0);
  OGRwkbGeometryType geomType = OGR_L_GetGeomType(layer);

  // TODO: Might be a little naïve ...
  switch(geomType) {
    case wkbUnknown:
      ret = -1;
      break;
    case wkbPoint:
    case wkbMultiPoint:
      ret = MS_LAYER_POINT;
      break;
    case wkbLineString:
    case wkbMultiLineString:
      ret = MS_LAYER_LINE;
      break;
    case wkbPolygon:
    case wkbMultiPolygon:
    case wkbGeometryCollection:
      ret = MS_LAYER_POLYGON;
      break;
    default:
      ret = -1;
  }
  OGRReleaseDataSource(hDS);

  return ret;
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
    /* data is a file - need to check if relative or absolute, if it exists ... */
    QFileInfo dataFinfo = QFileInfo(dataStr);
    if (dataFinfo.isRelative()) {
      dataStr = QFileInfo(qgsPath).dir().absolutePath() + "/" + dataStr;
    }
    // data is ogr, call the underlying library to determine the type
    int geomType = MS_LAYER_RASTER;
    if (typeStr == "ogr") {
      geomType = getGeometryType(dataStr);
    }

    mf->addLayer(layerName, dataStr, projStr, geomType);
  }



  f.close();

  return mf;
}
