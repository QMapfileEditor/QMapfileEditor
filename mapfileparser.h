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
    QVector<QString> * getLayers(void);

    int getMapExtentMinX();
    int getMapExtentMinY();
    int getMapExtentMaxX();
    int getMapExtentMaxY();

    bool isLoaded();
  private:
    // Private plain mapserver object
    struct mapObj * map = NULL;

    // accessible objects (via getters/setters)

    // map name and extent are directly accessible

    // layers list
    QVector<QString> * layers = NULL;

};

#endif // MAPFILEPARSER_H
