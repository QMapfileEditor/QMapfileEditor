#ifndef MAPFILEPARSER_H
#define MAPFILEPARSER_H

#include <QHash>
#include <QStringList>
#include <QList>

#include <string>
#include <iostream>



class MapfileParser
{
 public:
  MapfileParser(const QString & filename);
  ~MapfileParser();

  QString getMapName();
  QString getMapfilePath();
  QString getMapfileName();

  QStringList getLayers(void);

  bool getMapStatus();
  int getMapWidth();
  int getMapHeight();
  int getMapSize();
  int getMapUnits();
  QString getMapImageType();
  int getMapProjection();

  int getMapExtentMinX();
  int getMapExtentMinY();
  int getMapExtentMaxX();
  int getMapExtentMaxY();
  unsigned char * getCurrentMapImage();
  int getCurrentMapImageSize();
  bool saveMapfile(const QString & filename);

  bool getDebugStatus();
  int getDebug();
  QString getDebugFile();
  QString browseDebugFile();
  QString getShapepath();
  QString getSymbolSet();
  QString getFontSet();

  QString getTemplatePattern();
  QString getDataPattern();
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
  int getDefResolution();
  float getAngle();
  QList<int> getImageColor();

  bool isLoaded();
  bool isNew();

  int debug;

 private:
  // Private plain mapserver object
  struct mapObj * map = NULL;
  QString filename;

  // accessible objects (via getters/setters)
  struct imageObj * currentImage = NULL;
  int currentImageSize;
  // map name and extent are directly accessible

};

#endif // MAPFILEPARSER_H
