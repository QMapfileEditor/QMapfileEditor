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
MapfileParser::MapfileParser(const QString & fname) :
    filename(fname)
{
  this->map = umnms_new_map((char *) filename.toStdString().c_str());
  if (this->map == NULL) {
    return;
  }
  // Loads layers into an array of QString
  this->layers = new QVector<QString>();
  for (int i = 0; i <  this->map->numlayers ; i++) {
    QString curStr = QString(this->map->layers[i]->name);
    this->layers->append(curStr);
  }

  this->debug = this->map->debug;
}

bool MapfileParser::isNew()    { return (this->filename.isEmpty()); }
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

// Status parameters
bool MapfileParser::getMapStatus() {
  if (this->map)
    return this->map->status;
  return -1;
}

// Width/Height parameters
int MapfileParser::getMapWidth() {
  if (this->map)
    return this->map->width;
  return -1;
}

int MapfileParser::getMapHeight() {
  if (this->map)
    return this->map->height;
  return -1;
}

int MapfileParser::getMapSize() {
  if (this->map)
    return this->map->maxsize;
  return -1;
}

// units parameter
int MapfileParser::getMapUnits() {
  if (this->map)
    return this->map->units;
  return -1;
}

// imageType parameter
QString MapfileParser::getMapImageType() {
  if (this->map)
    return this->map->imagetype;
  return "";
}

//projection parameters
int MapfileParser::getMapProjection() {
  if (this->map)
    return this->map->projection.wellknownprojection;
  return -1;
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

bool MapfileParser::getDebugStatus() {
    if (this->map)
        return this->map->debug;
    return false;
}

QString MapfileParser::getDebugFile() {
    if (this->map)
        return msLookupHashTable( &(this->map->configoptions), "MS_ERRORFILE");
    return NULL;
}

QString MapfileParser::getMapfilePath() { return QString(this->map->mappath); }
QString MapfileParser::getMapfileName() { return QString(this->filename); }

bool MapfileParser::saveMapfile(const QString & filename) {
  int ret = -1;
  if (this->map) {
    // mapfile is a new one ("create mapfile" action)
    // filename argument should be "valid"  then
    if (! filename.isEmpty()) {
      ret = msSaveMap(this->map, (char *) filename.toStdString().c_str());
    }
    // using existing file (already existing mapfile loaded)
    // ("save" action)
    else if (! this->filename.isEmpty()) {
      ret = msSaveMap(this->map, (char *) filename.toStdString().c_str());
    }
  }
  return (ret == 0);
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
