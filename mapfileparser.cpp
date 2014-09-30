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
 * Loads a mapfile from an existing path, or from scratch, depending on the
 * value of the parameter fname.
 *
 * - if fname is empty, a new mapfile is asked for creation to libmapserver,
 *   preparing an object in memory.
 *
 * - if fname is not empty (and points to an existing path), then the
 *   libmapserver is asked to load a map object, relying on this existing
 *   mapfile.
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

  // issue #13:
  //
  // mapserver will internally adjust the extent when calling msDrawMap, since
  // we do not want to alter the original parameters, we need to save them, then
  // restore once called.
  //
  // See msAdjustExtent() in maputil.c for more info.

  double tmpXMax = this->map->extent.maxx,
         tmpXMin = this->map->extent.minx,
         tmpYMax = this->map->extent.maxy,
         tmpYMin = this->map->extent.miny;

  imageObj * ret = msDrawMap(this->map, MS_FALSE);

  this->map->extent.maxx = tmpXMax;
  this->map->extent.minx = tmpXMin;
  this->map->extent.maxy = tmpYMax;
  this->map->extent.miny = tmpYMin;

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

bool MapfileParser::setMapName( const QString & name) {
  if (this->map) {
    if (this->map->name) {
      free (this->map->name);
    }
    this->map->name = (char *) strdup(name.toStdString().c_str());

    return true;
  }
  return false;
}

// Status parameters
bool MapfileParser::getMapStatus() {
  if (this->map)
    return this->map->status;
  return -1;
}

bool MapfileParser::setMapStatus( const int & status) {
  if (this->map) {
    this->map->status = status;
    return true;
  }
  return false;
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

bool  MapfileParser::setMapSize(const int & width, const int & height) {
   if (this->map) {
    this->map->width = width;
    this->map->height = height;
    return true;
  }
  return false;
}

int MapfileParser::getMapMaxsize() {
  if (this->map)
    return this->map->maxsize;
  return -1;
}

bool MapfileParser::setMapMaxsize(const int & maxsize) {
  if (this->map) {
    this->map->maxsize = maxsize;
    return true;
  }
  return false;
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

bool MapfileParser::setMapExtent(const int & minx, const int & miny, const int & maxx, const int & maxy) {
  if (this->map) {
    this->map->extent.minx = minx;
    this->map->extent.miny = miny;
    this->map->extent.maxx = maxx;
    this->map->extent.maxy = maxy;
    return true;
  }
  return false;
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

QHash<QString, QString> MapfileParser::getMetadatas() {
  QHash<QString, QString> ret;
  if (! this->map) {
    return ret;
  }

  const char * tmpkey = msFirstKeyFromHashTable(& this->map->web.metadata);
  const char * tmpval;

  while ((tmpval = msLookupHashTable(& this->map->web.metadata, tmpkey))) {
      ret.insert(QString(tmpkey), QString(tmpval));
      // next key
      tmpkey = msNextKeyFromHashTable(& this->map->web.metadata, tmpkey);
  }

  return ret;
}


QString MapfileParser::getMetadataWmsTitle() {
    if (this->map) {
        if( msLookupHashTable( &(this->map->web.metadata), "WMS_TITLE")) {
            return msLookupHashTable( &(this->map->web.metadata), "WMS_TITLE");
        }
        if (msLookupHashTable( &(this->map->web.metadata), "OWS_TITLE")) {
            return msLookupHashTable( &(this->map->web.metadata), "OWS_TITLE");
        }
    }
    return NULL;
}
QString MapfileParser::getMetadataWfsTitle() {
    if (this->map) {
        if( msLookupHashTable( &(this->map->web.metadata), "WFS_TITLE")) {
            return msLookupHashTable( &(this->map->web.metadata), "WFS_TITLE");
        }
        if (msLookupHashTable( &(this->map->web.metadata), "OWS_TITLE")) {
            return msLookupHashTable( &(this->map->web.metadata), "OWS_TITLE");
        }
    }
    return NULL;
}

QString MapfileParser::getMetadataWmsOnlineresource() {
    if (this->map) {
        if( msLookupHashTable( &(this->map->web.metadata), "WMS_ONLINERESOURCE")) {
            return msLookupHashTable( &(this->map->web.metadata), "WMS_ONLINERESOURCE");
        }
        if (msLookupHashTable( &(this->map->web.metadata), "OWS_ONLINERESOURCE")) {
            return msLookupHashTable( &(this->map->web.metadata), "OWS_ONLINERESOURCE");
        }
    }
    return NULL;
}

QString MapfileParser::getMetadataWfsOnlineresource() {
    if (this->map) {
        if( msLookupHashTable( &(this->map->web.metadata), "WFS_ONLINERESOURCE")) {
            return msLookupHashTable( &(this->map->web.metadata), "WFS_ONLINERESOURCE");
        }
        if (msLookupHashTable( &(this->map->web.metadata), "OWS_ONLINERESOURCE")) {
            return msLookupHashTable( &(this->map->web.metadata), "OWS_ONLINERESOURCE");
        }
    }
    return NULL;
}

QString MapfileParser::getMetadataWmsSrs() {
    if (this->map) {
        if( msLookupHashTable( &(this->map->web.metadata), "WMS_SRS")) {
            return msLookupHashTable( &(this->map->web.metadata), "WMS_SRS");
        }
        if (msLookupHashTable( &(this->map->web.metadata), "OWS_SRS")) {
            return msLookupHashTable( &(this->map->web.metadata), "OWS_SRS");
        }
    }
    return NULL;
}
QString MapfileParser::getMetadataWfsSrs() {
    if (this->map) {
        if( msLookupHashTable( &(this->map->web.metadata), "WFS_SRS")) {
            return msLookupHashTable( &(this->map->web.metadata), "WFS_SRS");
        }
        if (msLookupHashTable( &(this->map->web.metadata), "OWS_SRS")) {
            return msLookupHashTable( &(this->map->web.metadata), "OWS_SRS");
        }
    }
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

QList<int> MapfileParser::getImageColor() {
   QList<int> color;

   if (this->map) {
     colorObj colorObj = this->map->imagecolor;
     color << colorObj.red << colorObj.green << colorObj.blue;
   }
   return color;
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
