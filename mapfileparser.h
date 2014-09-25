#ifndef MAPFILEPARSER_H
#define MAPFILEPARSER_H

#include <QVector>
#include <QString>

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

  QVector<QString> * getLayers(void);

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




  int getResolution();
  int getDefResolution();
  float getAngle();

  bool isLoaded();
  bool isNew();

  int debug;

 private:
  // Private plain mapserver object
  struct mapObj * map = NULL;
  QString filename;

  // accessible objects (via getters/setters)

  // map name and extent are directly accessible

  // layers list
  QVector<QString> * layers = NULL;

};

#endif // MAPFILEPARSER_H
