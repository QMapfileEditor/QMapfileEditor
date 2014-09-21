#include <mapserver.h>
#include <mapfile.h>

#include <string>
#include <iostream>


#include "mapfileparser.h"

/**
 *
 * This class provides a bridge between raw Mapserver C API and
 * our C++ application.
 *
 */

// Defining missing functions
// declared in mapserver-api
extern "C" {
  mapObj * umnms_new_map(char * filename) {
    mapObj *map = NULL;
    if(filename) {
      map = msLoadMap(filename,NULL);
    } else {
      map = (mapObj *)msSmallCalloc(sizeof(mapObj),1);
      if(initMap(map) == -1) {
        free(map);
        return NULL;
      }
    }
    return map;
  }
}


/**
 * Loads a mapfile from an existing path.
 *
 */
MapfileParser::MapfileParser(const std::string filename)
{
  this->map = umnms_new_map((char *) filename.c_str());
  if (this->map == NULL) {
    return;
  }
  this->filename = QString::fromStdString(filename);
  // Loads layers into an array of QString
  this->layers = new QVector<QString>();
  for (int i = 0; i <  this->map->numlayers ; i++) {
    QString curStr = QString(this->map->layers[i]->name);
    this->layers->append(curStr);
  }
}
/**
 * Creates a mapfile from scratch.
 */
MapfileParser::MapfileParser() {
  this->map = umnms_new_map(NULL);
}

bool MapfileParser::isLoaded() { return (this->map != NULL); }

// Layers
QVector<QString> * MapfileParser::getLayers() {
  return this->layers;
}
// Map name
QString MapfileParser::getMapName() {
  if (this->map)
    return QString(this->map->name);
  return QString("");
}

// Extent object parameters
int MapfileParser::getMapExtentMinX() {
  if (this->map)
    return this->map->extent.minx;
  return -1;
}

int MapfileParser::getMapExtentMinY() {
  if (this->map)
    return this->map->extent.miny;
  return -1;
}

int MapfileParser::getMapExtentMaxX() {
  if (this->map)
    return this->map->extent.maxx;
  return -1;
}

int MapfileParser::getMapExtentMaxY() {
  if (this->map)
    return this->map->extent.maxy;
  return -1;
}

QString MapfileParser::getMapfilePath() { return QString(this->map->mappath); }
QString MapfileParser::getMapfileName() { return QString(this->filename); }

int MapfileParser::saveMapfile() {
  if (this->map)
  {
    QByteArray byteArray = this->filename.toUtf8();
    const char* cString = byteArray.constData();
    return this->saveAsMapfile(cString);
  }
  return -1;
}

int MapfileParser::saveAsMapfile(const std::string filename) {
  if (this->map)
  {

    msSaveMap(this->map, (char *) filename.c_str());
    return 1;
  }
  return -1;
}

// Destructor

MapfileParser::~MapfileParser() {
  if (this->map) {
    free(this->map);
  }
  if (this->layers) {
    delete this->layers;
  }
};
