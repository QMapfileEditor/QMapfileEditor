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

#ifndef MAPFILEPARSER_H
#define MAPFILEPARSER_H

#include <QColor>
#include <QHash>
#include <QList>
#include <QStringList>

#include <string>
#include <iostream>

#include "outputformat.h"
#include "layer.h"

class MapfileParser
{
 public:
  MapfileParser(const QString & filename = "");
  ~MapfileParser();

  QString const & getMapName() const;
  void setMapName(const QString & name);

  QString const & getMapfilePath() const;
  QString const & getMapfileName() const;


  QList<Layer *> const & getLayers(void) const;

  QList<OutputFormat *> const & getOutputFormats(void) const;
  OutputFormat * getOutputFormat(const QString &);
  QString const & getDefaultOutputFormat(void) const;

  void addOutputFormat(OutputFormat * const of);
  void removeOutputFormat(OutputFormat * const of);
  void updateOutputFormat(OutputFormat * const of);

  void setDefaultOutputFormat(QString const &);

  bool const & getMapStatus() const;
  void setMapStatus(const bool & status);

  int  const & getMapWidth() const;
  int  const & getMapHeight() const;
  void setMapSize(const int & width, const int & height);

  int  const & getMapMaxsize() const;
  void setMapMaxsize(const int & maxsize);

  int  const & getMapUnits() const;
  void setMapUnits(const QString & units);
  void setMapUnits(int const & units);

  QString const & getMapImageType() const;
  void setMapImageType(const QString & imageType);

  QString const & getMapProjection() const;
  void setMapProjection(const QString & projection);

  double const & getMapExtentMinX() const;
  double const & getMapExtentMinY() const;
  double const & getMapExtentMaxX() const;
  double const & getMapExtentMaxY() const;
  void setMapExtent(const double & minx, const double & miny, const double & maxx, const double & maxy);

  unsigned char * getCurrentMapImage(const int & width = -1, const int & height = -1);
  int const     & getCurrentMapImageSize() const;

  bool saveMapfile(const QString & filename);

  int const & getDebug() const;
  void setDebug(const int & debug);

  QString const & getShapepath() const;
  void setShapepath(const QString & shapepath);

  QString const & getSymbolSet() const;
  void setSymbolSet(const QString & symbolset);

  QString const & getFontSet() const;
  void setFontSet(const QString & fontset);

  QString const & getTemplatePattern() const;
  void setTemplatePattern(const QString & pattern);

  QString const & getDataPattern() const;
  void setDataPattern(const QString & pattern);

  QString getMetadataWmsTitle();
  QString getMetadataWfsTitle();
  QString getMetadataWmsOnlineresource();
  QString getMetadataWfsOnlineresource();
  QString getMetadataWmsSrs();
  QString getMetadataWfsSrs();

  bool wfsGetCapabilitiesEnabled();
  bool wfsGetFeatureEnabled();
  bool wfsDescribeFeatureTypeEnabled();
  bool wmsGetMapEnabled();
  bool wmsGetLegendGraphicEnabled();
  bool wmsGetCapabilitiesEnabled();
  bool wmsGetFeatureInfoEnabled();

  QHash<QString, QString> const & getConfigOptions(void) const;
  QString const getConfigOption(const QString &) const;
  void setConfigOption(const QString & name, const QString & value);
  void removeConfigOption(const QString & name);

  QHash<QString, QString> const & getMetadatas(void) const;
  void setMetadata(const QString & name, const QString & value);
  QString const getMetadata(const QString & name) const;
  void removeMetadata(const QString &);

  double const & getResolution() const;
  void setResolution(const double & resolution);

  double const & getDefResolution() const;
  void setDefResolution(const double & resolution);

  float const & getAngle() const;
  void setAngle(const float & angle);

  QColor getImageColor() const;
  void setImageColor(QColor const &);

  bool isLoaded();
  bool isNew();

  // Layer related methods
  // needed by the interface (+) on mainwindow
  void addLayer();
  // needed by QGisImporter (might be dropped in the future though)
  void addLayer(QString const &, QString const &, QString const &, int);
  // needed by the QUndo Layer commands framework
  void addLayer(Layer const *);
  bool layerExists(QString const &);
  void removeLayer(Layer const *);
  void updateLayer(Layer const &);

  // constants (mainly used to fill in the interface forms)
  static QStringList drivers;
  static QStringList imageTypes;
  static QStringList missingData;
  static QStringList ogcMapOptions;
  static QStringList units;
  static QHash<QString, QStringList> imageModes;

  static QStringList defaultImageModes;
  static QStringList IMgdGifgdPng;
  static QStringList IMTemplateOgr;
  static QStringList IMGdal;

 private:
  // plain mapserver object
  struct mapObj * map = NULL;

  QString filename;
  unsigned char * currentImageBuffer = NULL;
  int currentImageSize;

  QHash<QString, QString> populateMapFromMs(void *);
  void insertIntoMsMap(void *, const QString &, const QString &);
  void removeFromMsMap(void *, const QString &);

  QString name;
  bool status;
  QString mapPath;
  QList<OutputFormat *> outputFormats;
  QString defaultOutputFormat;

  int width, height;
  int mapMaxSize;
  int mapUnits;

  QString mapImageType;
  QString mapProjection;

  double minx, miny, maxx, maxy;

  int debug;

  QHash<QString,QString> configOptions;
  QHash<QString,QString> metadatas;

  QString shapePath;
  QString fontSet;
  QString symbolSet;

  double resolution, defResolution;

  float angle;

  QString templatePattern;
  QString dataPattern;

  // Layers
  QList<Layer *> layers;

};

#endif // MAPFILEPARSER_H
