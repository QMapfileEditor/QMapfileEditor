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
  unsigned char * getCurrentMapImage();
  int getCurrentMapImageSize();
  bool saveMapfile(const QString & filename);

  bool isLoaded();
  bool isNew();

 private:
  // Private plain mapserver object
  struct mapObj * map = NULL;
  QString filename;

  // accessible objects (via getters/setters)
  struct imageObj * currentImage = NULL;
  int currentImageSize;
  // map name and extent are directly accessible

  // layers list
  QVector<QString> * layers = NULL;

};

#endif // MAPFILEPARSER_H
