#ifndef MAPFILEPARSER_H
#define MAPFILEPARSER_H

#include <QHash>
#include <QStringList>
#include <QList>

#include <string>
#include <iostream>

#include "outputformat.h"

class MapfileParser
{
 public:
  MapfileParser(const QString & filename = "");
  ~MapfileParser();

  QString getMapName();
  void setMapName(const QString & name);
  QString getMapfilePath();
  QString getMapfileName();

  void addLayer(QString const &, QString const &, QString const &);

  QStringList getLayers(void);
  QStringList getGdalGdalDrivers(void) { return gdalGdalDrivers; };
  QStringList getGdalOgrDrivers(void) { return gdalOgrDrivers; };

  QList<OutputFormat *> getOutputFormats(void);
  void setOutputFormats(QList<OutputFormat *> const & of);
  QString const & getDefaultOutputFormat(void);
  void setDefaultOutputFormat(QString const &);

  bool getMapStatus();
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

  int  getMapProjection();
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
  void removeMetadata(const QString &);

  double getResolution();
  void   setResolution(const double & resolution);

  double getDefResolution();
  void   setDefResolution(const double & resolution);

  float getAngle();
  void setAngle(const float & angle);

  QList<int> getImageColor();
  void setImageColor(const int & red, const int & green, const int & blue);

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


};

#endif // MAPFILEPARSER_H
