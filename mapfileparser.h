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
  bool setMapName(const QString & name);
  QString getMapfilePath();
  QString getMapfileName();

  QStringList getLayers(void);
  QStringList getGdalGdalDrivers(void) { return gdalGdalDrivers; };
  QStringList getGdalOgrDrivers(void) { return gdalOgrDrivers; };

  QList<OutputFormat *> * getOutputFormats(void);
  OutputFormat * getOutputFormat(const QString &);

  bool getMapStatus();
  bool setMapStatus(const int & status);
  int getMapWidth();
  int getMapHeight();
  bool setMapSize(const int & width, const int & height);
  int getMapMaxsize();
  bool setMapMaxsize(const int & maxsize);
  int getMapUnits();
  bool setMapUnits(const QString & units);
  QString getMapImageType();
  bool setMapImageType(const QString & imageType);
  int getMapProjection();
  bool setMapProjection(const QString & projection);

  int getMapExtentMinX();
  int getMapExtentMinY();
  int getMapExtentMaxX();
  int getMapExtentMaxY();
  bool setMapExtent(const int & minx, const int & miny, const int & maxx, const int & maxy);
  unsigned char * getCurrentMapImage(const int & width = -1, const int & height = -1);
  int getCurrentMapImageSize();
  bool saveMapfile(const QString & filename);

  bool getDebugStatus();
  int getDebug();
  bool setDebug(const int & debug);
  QString getDebugFile();
  bool setMetadata(const QString & name, const QString & value);
  QString getShapepath();
  bool setShapepath(const QString & shapepath);
  QString getSymbolSet();
  bool setSymbolSet(const QString & symbolset);
  QString getFontSet();
  bool setFontSet(const QString & fontset);

  QString getTemplatePattern();
  bool setTemplatePattern(const QString & pattern);
  QString getDataPattern();
  bool setDataPattern(const QString & pattern);
  QString getConfigContextUrl();
  QString getConfigEncryptionKey();

  QString getConfigNonsquare();
  QString getConfigMissingData();
  QString getConfigProjLib();

  QString getMetadataWmsTitle();
  QString getMetadataWfsTitle();
  QString getMetadataWmsOnlineresource();
  QString getMetadataWfsOnlineresource();
  QString getMetadataWmsSrs();
  QString getMetadataWfsSrs();

  QHash<QString, QString> getMetadatas();


  int getResolution();
  bool setResolution(const int & resolution);
  int getDefResolution();
  bool setDefResolution(const int & resolution);
  float getAngle();
  bool setAngle(const int & angle);

  QList<int> getImageColor();
  bool setImageColor( const int & red, const int & green, const int & blue);

  bool isLoaded();
  bool isNew();

  int debug;

 private:
  // Private plain mapserver object
  struct mapObj * map = NULL;
  QString filename;
  QStringList gdalGdalDrivers;
  QStringList gdalOgrDrivers;
  QStringList units;
  QStringList imageTypes;
  QStringList missingData;
  QStringList ogcMapOptions;

  QList<OutputFormat *> * outputformats;

  // accessible objects (via getters/setters)
  struct imageObj * currentImage = NULL;
  int currentImageSize;
  // map name and extent are directly accessible

};

#endif // MAPFILEPARSER_H
