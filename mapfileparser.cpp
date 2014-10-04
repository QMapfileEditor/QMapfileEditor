#include <mapserver.h>
#include <mapfile.h>
#include <gdal.h>

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
  this->units << "inches" << "feet" << "miles" << "meters" << "kilometers" << 
       "dd" << "pixels" << "pourcentages" << "nauticalmiles";
  this->imageTypes << "jpeg" << "pdf" << "png" << "svg";
  this->missingData << "" << "FAIL" << "LOG" << "IGNORE";
  this->ogcMapOptions << "" << "ows_http_max_age" << "ows_schemas_location" << 
        "ows_sld_enabled" << "ows_updatesequence" << "wms_abstract" <<
        "wms_accessconstraints" << "wms_addresstype" << "wms_address" <<
        "wms_city" << "wms_stateorprovince" << "wms_postcode" << "wms_country" << 
        "wms_attribution_logourl_format" <<
        "wms_attribution_logourl_height" << "wms_attribution_logourl_href" <<
        "wms_attribution_logourl_width" << "wms_attribution_onlineresource" <<
        "wms_attribution_title" << "wms_bbox_extended" <<
        "wms_contactelectronicmailaddress" << "wms_contactfacsimiletelephone" << 
        "wms_contactperson" << "wms_contactorganization" <<
        "wms_contactposition" << "wms_contactvoicetelephone" << "wms_encoding" << 
        "wms_feature_info_mime_type" << "wms_fees" <<
        "wms_getcapabilities_version" << "wms_getlegendgraphic_formatlist" <<
        "wms_getmap_formatlist" << "wms_keywordlist" <<
        "wms_keywordlist_vocabulary" << "wms_keywordlist_[vocabulary name]_items" <<
        "wms_languages" << "wms_layerlimit" << "wms_resx" <<
        "wms_resy" << "wms_rootlayer_abstract" << "wms_rootlayer_keywordlist" << 
        "wms_rootlayer_title" << "wms_service_onlineresource" <<
        "wms_timeformat" << "ows_schemas_location" << "ows_updatesequence" <<
        "wfs_abstract" << "wfs_accessconstraints" << "wfs_encoding" <<
        "wfs_feature_collection" << "wfs_fees" <<
        "wfs_getcapabilities_version" << "wfs_keywordlist" <<
        "wfs_maxfeatures" << "wfs_namespace_prefix" << "wfs_namespace_uri" <<
        "wfs_service_onlineresource";

  // TODO: might be relevant to have this calculated
  // on the mainwindow object instead of having it here,
  // and calculate each time we create a new mapfile project.
  this->gdalGdalDrivers.clear();
  this->gdalOgrDrivers.clear();
  this->gdalGdalDrivers << "";
  this->gdalOgrDrivers << "";

  for (int i = 0; i < GDALGetDriverCount(); ++i) {
    GDALDriverH d = GDALGetDriver(i);
    this->gdalGdalDrivers << GDALGetDriverShortName(d) ;
  }
  for (int i = 0; i < OGRGetDriverCount(); ++i) {
    OGRSFDriverH d = OGRGetDriver(i);
    this->gdalOgrDrivers << OGR_Dr_GetName(d);
  }
  this->gdalOgrDrivers.removeDuplicates();
  this->gdalOgrDrivers.sort();
  this->gdalGdalDrivers.removeDuplicates();
  this->gdalGdalDrivers.sort();

  this->outputformats = new QList<OutputFormat *>();

  if (this->map) {
    for (int i = 0; i < this->map->numoutputformats ; i++) {
      this->outputformats->append(new OutputFormat(this->map->outputformatlist[i]->name,
                                                   this->map->outputformatlist[i]->mimetype,
                                                   this->map->outputformatlist[i]->driver,
                                                   this->map->outputformatlist[i]->extension,
                                                   this->map->outputformatlist[i]->imagemode,
                                                   this->map->outputformatlist[i]->transparent));
    }
  }
}

/**
 * Creates an image representation of the current map
 */
unsigned char * MapfileParser::getCurrentMapImage(const int & width, const int & height) {
  if (! this->map) {
    return NULL;
  }

  // invalidates previous data
  if (this->currentImageBuffer) {
    free(this->currentImageBuffer);
    this->currentImageBuffer = NULL;
    this->currentImageSize = 0;
  }

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

  // set querymap options to be coherent with the interface state
  imageObj * img = NULL;

  if (width > 0 && height > 0) {
    this->map->querymap.width  = width;
    this->map->querymap.height = height;
    img = msDrawMap(this->map, MS_TRUE);
  } else {
    img = msDrawMap(this->map, MS_FALSE);
  }

  this->map->extent.maxx = tmpXMax;
  this->map->extent.minx = tmpXMin;
  this->map->extent.maxy = tmpYMax;
  this->map->extent.miny = tmpYMin;

  if (img != NULL) {
    this->currentImageBuffer = msSaveImageBuffer(img, & this->currentImageSize, img->format);
    // we do not need img anymore
    msFreeImage(img);

    return this->currentImageBuffer;
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

//TODO
bool MapfileParser::setMapUnits(const QString & units) {
  if (this->map) {
      //TODO: needs to use MS_UNITS type
      //this->map->units = this->units.indexOf(units);
      return true;
  }
  return false;
}
// imageType parameter
QString MapfileParser::getMapImageType() {
  if (this->map)
    return this->map->imagetype;
  return "";
}

//TODO
bool MapfileParser::setMapImageType( const QString & imageType) {
    if (this->map) {
        return true;
    }
    return false;
}

//projection parameters
int MapfileParser::getMapProjection() {
  if (this->map)
    return this->map->projection.wellknownprojection;
  return -1;
}

//TODO: not correct, need to manage if startwith epsg, if so add +init=
bool MapfileParser::setMapProjection(const QString & projection) {
    if (this->map) {
        if (this->map->projection.wellknownprojection) {
            this->map->projection.wellknownprojection = 0;
        }
        //this->map->projection = (char *) strdup(projection.toString().c_str());
        return true;
    }
    return false;
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
//TODO: not working
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

bool MapfileParser::setDebug(const int & debug) {
    if(this->map) {
        this->map->debug = debug;
        return true;
    }
    return false;
}

QString MapfileParser::getDebugFile() {
    if (this->map)
        return msLookupHashTable( &(this->map->configoptions), "MS_ERRORFILE");
    return NULL;
}

QString MapfileParser::getConfigMissingData() {
    if (this->map)
        return msLookupHashTable( &(this->map->configoptions), "ON_MISSING_DATA");
    return NULL;
}

QString MapfileParser::getShapepath() {
    if (this->map)
        return this->map->shapepath;
    return NULL;
}

bool MapfileParser::setShapepath(const QString & shapepath) {
    if (this->map) {
        if (this->map->shapepath) {
            free(this->map->shapepath);
        }
        this->map->shapepath = (char *) strdup(shapepath.toStdString().c_str());
        return true;
    }
    return false;
}

QString MapfileParser::getSymbolSet() {
    if (this->map)
        return this->map->symbolset.filename;
    return NULL;
}

bool MapfileParser::setSymbolSet(const QString & symbolset) {
    if (this->map) {
        if (this->map->symbolset.filename) {
            free(this->map->symbolset.filename);
        }
        this->map->symbolset.filename = (char *) strdup(symbolset.toStdString().c_str());
        return true;
    }
    return false;
}

QString MapfileParser::getFontSet() {
    if (this->map)
        return this->map->fontset.filename;
    return NULL;
}

bool MapfileParser::setFontSet(const QString & fontset) {
    if (this->map) {
        if (this->map->fontset.filename) {
            free(this->map->fontset.filename);
        }
        this->map->fontset.filename = (char *) strdup(fontset.toStdString().c_str());
        return true;
    }
    return false;
}


int MapfileParser::getResolution() {
    if (this->map)
        return this->map->resolution;
    return -1;
}

bool MapfileParser::setResolution(const int & resolution) {
    if (this->map) {
        this->map->resolution = resolution;
        return true;
    }
    return false;
}

int MapfileParser::getDefResolution() {
    if (this->map)
        return this->map->defresolution;
    return -1;
}

bool MapfileParser::setDefResolution(const int & resolution) {
    if (this->map) {
        this->map->defresolution = resolution;
        return true;
    }
    return false;
}

float MapfileParser::getAngle() {
    if (this->map)
        return this->map->gt.rotation_angle;
    return -1.0;
}

bool MapfileParser::setAngle( const int & angle) {
    if (this->map) {
        this->map->gt.rotation_angle = angle;
        return true;
    }
    return false;
}

QString MapfileParser::getTemplatePattern() {
    if (this->map)
        return this->map->templatepattern;
    return NULL;
}

bool MapfileParser::setTemplatePattern(const QString & pattern) {
    if (this->map) {
        if (this->map->templatepattern) {
            free(this->map->templatepattern);
        }
        this->map->templatepattern = (char *) strdup(pattern.toStdString().c_str());
        return true;
    }
    return false;
}

QString MapfileParser::getDataPattern() {
    if (this->map)
        return this->map->datapattern;
    return NULL;
}

bool MapfileParser::setDataPattern(const QString & pattern) {
    if (this->map) {
        if (this->map->datapattern) {
            free(this->map->datapattern);
        }
        this->map->datapattern = (char *) strdup(pattern.toStdString().c_str());
        return true;
    }
    return false;
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

//TODO: make a method using haskTable?
bool MapfileParser::setMetadata(const QString & name, const QString & value) {
    if (this->map) {
        //this->map->metadata->msHashTable(name, value);
        return true;
    }
    return false;
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

QList<int> MapfileParser::getImageColor() {
   QList<int> color;

   if (this->map) {
     colorObj colorObj = this->map->imagecolor;
     color << colorObj.red << colorObj.green << colorObj.blue;
   }
   return color;
}

bool MapfileParser::setImageColor(const int & red, const int & green, const int & blue ) {
    if (this->map) {
        this->map->imagecolor.red = red;
        this->map->imagecolor.green = green;
        this->map->imagecolor.blue = blue;
        return true;
    }
    return false;
}

QList<OutputFormat *> * MapfileParser::getOutputFormats() {
  return this->outputformats;
}

OutputFormat * MapfileParser::getOutputFormat(const QString & key) {
  for (int i = 0; i < this->outputformats->size(); ++i) {
    if (key == this->outputformats->at(i)->getName())
      return this->outputformats->at(i);
  }
  return NULL;
}

MapfileParser::~MapfileParser() {
  if (this->map) {
    msFreeMap(this->map);
  }
  if (this->currentImageBuffer) {
    free(this->currentImageBuffer);
  }
  if (this->outputformats) {
    for (int i = 0 ; i < this->outputformats->size(); ++i) {
      delete this->outputformats->at(i);
    }
    delete this->outputformats;
  }
}
