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

class MapfileParser
{
 public:
  MapfileParser(const QString & filename = "");
  ~MapfileParser();

  QString const & getMapName() const;
  void setMapName(const QString & name);

  QString const & getMapfilePath() const;
  QString const & getMapfileName() const;

  void addLayer(QString const &, QString const &, QString const &, int);

  QStringList const & getLayers(void) const;

  QStringList const & getGdalGdalDrivers(void) const { return gdalGdalDrivers; };
  QStringList const & getGdalOgrDrivers(void)  const { return gdalOgrDrivers;  };


  QList<OutputFormat *> const & getOutputFormats(void) const;
  OutputFormat * getOutputFormat(const QString &);
  QString const & getDefaultOutputFormat(void) const;

  void addOutputFormat(OutputFormat * const of);
  void removeOutputFormat(OutputFormat const * of);
  void updateOutputFormat(OutputFormat * const of);

  void setDefaultOutputFormat(QString const &);

  bool const & getMapStatus() const;
  void setMapStatus(const bool & status);

  int  getMapWidth();
  int  getMapHeight();
  void setMapSize(const int & width, const int & height);

  int  getMapMaxsize();
  void setMapMaxsize(const int & maxsize);

  int  getMapUnits();
  void setMapUnits(const QString & units);
  void setMapUnits(int const & units);

  QString getMapImageType();
  void    setMapImageType(const QString & imageType);

  QString getMapProjection();
  void setMapProjection(const QString & projection);

  double getMapExtentMinX();
  double getMapExtentMinY();
  double getMapExtentMaxX();
  double getMapExtentMaxY();
  void setMapExtent(const double & minx, const double & miny, const double & maxx, const double & maxy);

  unsigned char * getCurrentMapImage(const int & width = -1, const int & height = -1);
  int             getCurrentMapImageSize();

  bool saveMapfile(const QString & filename);

  int   getDebug();
  void  setDebug(const int & debug);



  QString getShapepath();
  void    setShapepath(const QString & shapepath);

  QString getSymbolSet();
  void    setSymbolSet(const QString & symbolset);

  QString getFontSet();
  void    setFontSet(const QString & fontset);

  QString getTemplatePattern();
  void    setTemplatePattern(const QString & pattern);

  QString getDataPattern();
  void    setDataPattern(const QString & pattern);

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

  QHash<QString, QString> getConfigOptions(void);
  QString                 getConfigOption(const QString &);
  void                    setConfigOption(const QString & name, const QString & value);
  void                    removeConfigOption(const QString & name);

  QHash<QString, QString> getMetadatas(void);
  void setMetadata(const QString & name, const QString & value);
  QString getMetadata(const QString & name);
  void removeMetadata(const QString &);

  double getResolution();
  void   setResolution(const double & resolution);

  double getDefResolution();
  void   setDefResolution(const double & resolution);

  float getAngle();
  void setAngle(const float & angle);

  QColor getImageColor() const;
  void setImageColor(QColor const &);

  bool isLoaded();
  bool isNew();

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
  // Private plain mapserver object
  struct mapObj * map = NULL;

  QString filename;
  QStringList gdalGdalDrivers;
  QStringList gdalOgrDrivers;

  unsigned char * currentImageBuffer = NULL;
  int currentImageSize;

  QHash<QString, QString> populateMapFromMs(void *);
  void insertIntoMsMap(void *, const QString &, const QString &);
  void removeFromMsMap(void *, const QString &);

  QString name;
  QStringList layers;
  bool status;
  QString mapPath;
  QList<OutputFormat *> outputFormats;
  QString defaultOutputFormat;


};

#endif // MAPFILEPARSER_H
