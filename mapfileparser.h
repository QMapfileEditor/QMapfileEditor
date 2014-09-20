#ifndef MAPFILEPARSER_H
#define MAPFILEPARSER_H

#include <QVector>
#include <QString>

#include <string>
#include <iostream>



class MapfileParser
{
  public:
    MapfileParser(void);
    MapfileParser(const std::string filename);
    ~MapfileParser();

      QString getMapName();
      QString getMapfilePath();
      QString getMapfileName();
      
      QVector<QString> * getLayers(void);

      int getMapExtentMinX();
      int getMapExtentMinY();
      int getMapExtentMaxX();
      int getMapExtentMaxY();
      int saveAsMapfile(const std::string filename);
      int saveMapfile();

    bool isLoaded();
  private:
      // Private plain mapserver object
      struct mapObj * map = NULL;
      QString filename ;
      
      // accessible objects (via getters/setters)

    // map name and extent are directly accessible

    // layers list
    QVector<QString> * layers = NULL;

};

#endif // MAPFILEPARSER_H
