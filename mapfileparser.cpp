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
    filename(fname), currentImageSize(0)
{
  this->map = umnms_new_map(fname.isEmpty() ? NULL :  (char *) filename.toStdString().c_str());
  if (this->map == NULL) {
    return;
  }
}

/**
 * Creates an image representation of the current map
 */
unsigned char * MapfileParser::getCurrentMapImage() {
  if (! this->map)
    return NULL;
  // image already loaded
  if (this->currentImage)
    return this->currentImage->img.raw_byte;

  // TODO: ERROR HERE, leaking memory ...
  // just meant to try out
  imageObj * ret = msDrawMap(this->map, MS_FALSE);
  if (ret != NULL) {
    this->currentImage = ret;
    return msSaveImageBuffer(this->currentImage, &this->currentImageSize, this->currentImage->format);
  }
  return NULL;
}

int MapfileParser::getCurrentMapImageSize() {
  return this->currentImageSize;
}

bool MapfileParser::isNew()    { return (this->filename.isEmpty()); }
bool MapfileParser::isLoaded() { return (this->map != NULL); }

// get layers
QStringList MapfileParser::getLayers() {
  QStringList ret = QStringList();
  if (this->map) {
    for (int i = 0; i <  this->map->numlayers ; i++) {
      ret << this->map->layers[i]->name;
    }
  }
  return ret;
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

int MapfileParser::getDebug() {
    if (this->map)
        return this->map->debug;
    return false;
}

QString MapfileParser::getDebugFile() {
    if (this->map)
        return msLookupHashTable( &(this->map->configoptions), "MS_ERRORFILE");
    return NULL;
}

QString MapfileParser::getShapepath() {
    if (this->map)
        return this->map->shapepath;
    return NULL;
}

QString MapfileParser::getSymbolSet() {
    if (this->map)
        return this->map->symbolset.filename;
    return NULL;
}

QString MapfileParser::getFontSet() {
    if (this->map)
        return this->map->fontset.filename;
    return NULL;
}

int MapfileParser::getResolution() {
    if (this->map)
        return this->map->resolution;
    return -1;
}

int MapfileParser::getDefResolution() {
    if (this->map)
        return this->map->defresolution;
    return -1;
}

float MapfileParser::getAngle() {
    if (this->map)
        return this->map->gt.rotation_angle;
    return -1.0;
}

QString MapfileParser::getTemplatePattern() {
    if (this->map)
        return this->map->templatepattern;
    return NULL;
}

QString MapfileParser::getDataPattern() {
    if (this->map)
        return this->map->datapattern;
    return NULL;
}

QString MapfileParser::getConfigContextUrl() {
    if (this->map)
        return  msLookupHashTable( &(this->map->configoptions), "CGI_CONTEXT_URL");
    return NULL;
}

QString MapfileParser::getConfigEncryptionKey() {
    if (this->map)
        return msLookupHashTable( &(this->map->configoptions), "MS_ENCRYPTION_KEY");
    return NULL;
}

QString MapfileParser::getConfigNonsquare() {
    if (this->map)
        return msLookupHashTable( &(this->map->configoptions), "MS_NONSQUARE");
    return NULL;
}

QString MapfileParser::getConfigMissingData() {
    if (this->map)
        return msLookupHashTable( &(this->map->configoptions), "ON_MISSING_DATA");
    return NULL;
}
QString MapfileParser::getConfigProjLib() {
    if (this->map)
        return msLookupHashTable( &(this->map->configoptions), "PROJ_LIB");
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

QString MapfileParser::browseDebugFile() {
   return "";
}

// Destructor

MapfileParser::~MapfileParser() {
  if (this->map) {
    msFreeMap(this->map);
  }
  if (this->currentImage) {
    msFreeImage(this->currentImage);
  }
}
