/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: 
 * Author: Pierre Mauduit
 *
 **********************************************************************
 * Copyright (c) 2014, Pierre Mauduit
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of this Software or works derived from this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 ****************************************************************************/

#include <mapserver.h>
#include <mapfile.h>
#include <gdal.h>

#include <string>
#include <iostream>

#include <QDebug>

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

  this->name = QString();
  this->layers = QStringList();
  this->status = false;
  this->mapPath = QString();
  this->outputFormats = QList<OutputFormat *>();
  this->defaultOutputFormat = QString();

  if (this->map) {
    // name
    this->name = QString(this->map->name);
    // Layers
    // TODO: should become QList<Layer *>
    // leaving it as is for now, for compatibility
    for (int i = 0; i <  this->map->numlayers ; i++) {
      this->layers << this->map->layers[i]->name;
    }
    // status
    this->status = this->map->status;
    // map path
    this->mapPath = QString(this->map->mappath);
    // output formats
    for (int i = 0; i < this->map->numoutputformats ; i++) {
      OutputFormat * item = new OutputFormat(this->map->outputformatlist[i]->name,
                                             this->map->outputformatlist[i]->mimetype,
                                             this->map->outputformatlist[i]->driver,
                                             this->map->outputformatlist[i]->extension,
                                             this->map->outputformatlist[i]->imagemode,
                                             this->map->outputformatlist[i]->transparent,
                                             OutputFormat::UNCHANGED);

      for (int j = 0 ; j < this->map->outputformatlist[i]->numformatoptions; ++j) {
        QStringList kv = QString(this->map->outputformatlist[i]->formatoptions[j]).split("=");
        if (kv.size() == 2)
          item->addFormatOption(kv[0], kv[1]);
      }
      this->outputFormats.append(item);
    } // output formats

   // default output format
   this->defaultOutputFormat = QString(this->map->imagetype);

  }
}

/**
 * Related method to create an image representation of the current map
 */
int MapfileParser::getCurrentMapImageSize() {
  return this->currentImageSize;
}

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


bool MapfileParser::isNew()    { return (this->filename.isEmpty()); }
bool MapfileParser::isLoaded() { return (this->map != NULL); }


// Layers-related methods

QStringList const & MapfileParser::getLayers() const {
  return layers;
}

void MapfileParser::addLayer(QString const & layerName, QString const & dataStr, QString const & projStr, int geomType) {
  layerObj *newLayer =  (layerObj *) malloc(sizeof(layerObj));
  initLayer(newLayer, this->map);
  // TODO: check if unique before doing this
  if (newLayer->name)
    free(newLayer->name);

  newLayer->name = strdup(layerName.toStdString().c_str());

  // TODO: relative / absolute path ?
  if (newLayer->data)
    free(newLayer->data);
  newLayer->data = strdup(dataStr.toStdString().c_str());

  newLayer->type = (MS_LAYER_TYPE) geomType;

  msLoadProjectionStringEPSG(& (newLayer->projection), projStr.toStdString().c_str());

  // inserts the layer at the end
  msInsertLayer(this->map, newLayer, -1);
}



// Map name
QString const & MapfileParser::getMapName() const {
  return name;
}

void MapfileParser::setMapName(const QString & name) {
  if (this->map) {
    if (this->map->name) {
      free (this->map->name);
    }
    this->map->name = (char *) strdup(name.toStdString().c_str());
    this->name = name;
  }
}

// Status parameters
bool const & MapfileParser::getMapStatus() const {
  return status;
}

void MapfileParser::setMapStatus(const bool & status) {
  if (this->map) {
    this->map->status = (status == true ? 1 : 0);
    this->status = status;
  }
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

void MapfileParser::setMapSize(const int & width, const int & height) {
   if (this->map) {
    this->map->width = width;
    this->map->height = height;
  }
}

int MapfileParser::getMapMaxsize() {
  if (this->map)
    return this->map->maxsize;
  return -1;
}

void MapfileParser::setMapMaxsize(const int & maxsize) {
  if (this->map) {
    this->map->maxsize = maxsize;
  }
}

// units parameter
int MapfileParser::getMapUnits() {
  if (this->map)
    return this->map->units;
  return -1;
}

void MapfileParser::setMapUnits(const QString & units) {
  if (this->map) {
    this->map->units = (enum MS_UNITS) this->units.indexOf(units);
  }
}
void MapfileParser::setMapUnits(int const & units) {
  if (this->map) {
    this->map->units = (enum MS_UNITS) units;
  }
}


// imageType parameter
QString MapfileParser::getMapImageType() {
  if (this->map)
    return this->map->imagetype;
  return QString();
}

void MapfileParser::setMapImageType(const QString & imageType) {
  if (! this->map)
    return;
  if (this->map->imagetype) {
    free(this->map->imagetype);
  }
  this->map->imagetype = (char *) strdup(imageType.toStdString().c_str());
}

//projection parameters
QString MapfileParser::getMapProjection() {
  QString ret = QString();
  if (this->map) {
    char * tmp = msGetProjectionString(& (this->map->projection));
    ret = QString(tmp);
    free(tmp);
    return ret;
  }
  return QString();
}

void MapfileParser::setMapProjection(const QString & projection) {
    if (this->map) {
      msLoadProjectionStringEPSG(& (this->map->projection), projection.toStdString().c_str());
    }
}

// Extent object parameters
double MapfileParser::getMapExtentMinX() {
  if (this->map)
    return this->map->extent.minx;
  return -1;
}

double MapfileParser::getMapExtentMinY() {
  if (this->map)
    return this->map->extent.miny;
  return -1;
}

double MapfileParser::getMapExtentMaxX() {
  if (this->map)
    return this->map->extent.maxx;
  return -1;
}

double MapfileParser::getMapExtentMaxY() {
  if (this->map)
    return this->map->extent.maxy;
  return -1;
}

void MapfileParser::setMapExtent(const double & minx, const double & miny, const double & maxx, const double & maxy) {
  if (this->map) {
    this->map->extent.minx = minx;
    this->map->extent.miny = miny;
    this->map->extent.maxx = maxx;
    this->map->extent.maxy = maxy;
  }
}

int MapfileParser::getDebug() {
    if (this->map)
        return this->map->debug;
    return false;
}

void MapfileParser::setDebug(const int & debug) {
    if(this->map) {
        this->map->debug = debug;
    }
}

/**
 * Private methods allowing the mapping between hashtables from MS and Qt
 * objects:
 *
 * - populateMapFromMs does the translation between Mapserver internals
 *   and plain Qt objects.
 *
 * - insertIntoMsMap permits to create or update a record in the hashtable
 *   mapserver side.
 *
 * - removeFromMsMap permits to remove an entry from the MS hashtables.
 */
QHash<QString, QString> MapfileParser::populateMapFromMs(void *table) {

  QHash<QString, QString> ret = QHash<QString, QString> ();

  const char * tmpkey = msFirstKeyFromHashTable((hashTableObj *) table);
  const char * tmpval;

  while ((tmpval = msLookupHashTable((hashTableObj *) table, tmpkey))) {
      ret.insert(QString(tmpkey), QString(tmpval));
      tmpkey = msNextKeyFromHashTable((hashTableObj *) table, tmpkey);
  }
  return ret;
}

void MapfileParser::insertIntoMsMap(void *table, const QString &name, const QString &value) {
  // - msInsertHashTable replaces the current element (memory is freed).
  // - the value is strdup'ed.
  //
  // So it should be safe to pass std::string's char* pointers.
  msInsertHashTable((hashTableObj *) table, name.toStdString().c_str(), value.toStdString().c_str());
}

void MapfileParser::removeFromMsMap(void *table, const QString &name) {
  msRemoveHashTable((hashTableObj *) table, name.toStdString().c_str());
}


/**
 * map configuration options (nested into map->configoptions)
 */
QHash<QString, QString> MapfileParser::getConfigOptions() {
  if (! this->map)
    return QHash<QString, QString>();
  // TODO: same remark as for metadatas
  return populateMapFromMs(& (this->map->configoptions));
}

QString MapfileParser::getConfigOption(const QString &key) {
  return this->getConfigOptions().value(key, QString());
}

void MapfileParser::setConfigOption(const QString & name, const QString & value) {
  if (! this->map)
    return;
  // Doing nothing if current config option is already set to the given value
  if (this->getConfigOptions().value(name) == value)
    return;
  insertIntoMsMap(& (this->map->configoptions), name, value);
}

void MapfileParser::removeConfigOption(const QString & name) {
  if (! this->map)
    return;
  removeFromMsMap(& (this->map->configoptions), name);
}

/**
 * metadatas (nested into map->web).
 */

QHash<QString, QString> MapfileParser::getMetadatas() {
  if (! this->map)
    return QHash<QString, QString>();
  // TODO: this should be done only once, and saved
  // into a member variable.
  return populateMapFromMs(& (this->map->web.metadata));
}

QString MapfileParser::getMetadata(const QString & name) {
  return this->getMetadatas().value(name);
}

void MapfileParser::setMetadata(const QString & name, const QString & value) {
  if (! this->map)
    return;
  if (this->getMetadatas().value(name) == value)
    return;
  insertIntoMsMap(& (this->map->web.metadata), name, value);
}

void MapfileParser::removeMetadata(const QString & name) {
  if (! this->map)
    return;
  removeFromMsMap(& (this->map->web.metadata), name);
}

// WFS operations

bool MapfileParser::wfsGetCapabilitiesEnabled() {
  return (((this->getMetadatas().value("wfs_enable_request").contains("*") ||
       (this->getMetadatas().value("wfs_enable_request").contains("GetCapabilities")))
      && (! this->getMetadatas().value("wfs_enable_request").contains("!GetCapabilities")))
    ||
    ((this->getMetadatas().value("ows_enable_request").contains("*") ||
       (this->getMetadatas().value("ows_enable_request").contains("GetCapabilities")))
      && (! this->getMetadatas().value("ows_enable_request").contains("!GetCapabilities"))));
}

bool MapfileParser::wfsGetFeatureEnabled() {
  return (((this->getMetadatas().value("wfs_enable_request").contains("*") ||
       (this->getMetadatas().value("wfs_enable_request").contains("GetFeature")))
      && (! this->getMetadatas().value("wfs_enable_request").contains("!GetFeature")))
    ||
    ((this->getMetadatas().value("ows_enable_request").contains("*") ||
       (this->getMetadatas().value("ows_enable_request").contains("GetFeature")))
      && (! this->getMetadatas().value("ows_enable_request").contains("!GetFeature"))));
}

bool MapfileParser::wfsDescribeFeatureTypeEnabled() {
  return (((this->getMetadatas().value("wfs_enable_request").contains("*") ||
       (this->getMetadatas().value("wfs_enable_request").contains("DescribeFeatureType")))
      && (! this->getMetadatas().value("wfs_enable_request").contains("!DescribeFeatureType")))
    ||
    ((this->getMetadatas().value("ows_enable_request").contains("*") ||
       (this->getMetadatas().value("ows_enable_request").contains("DescribeFeatureType")))
      && (! this->getMetadatas().value("ows_enable_request").contains("!DescribeFeatureType"))));
}

// WMS operations

bool MapfileParser::wmsGetMapEnabled() {
  return (((this->getMetadatas().value("wms_enable_request").contains("*") ||
       (this->getMetadatas().value("wms_enable_request").contains("GetMap")))
      && (! this->getMetadatas().value("wms_enable_request").contains("!GetMap")))
    ||
    ((this->getMetadatas().value("ows_enable_request").contains("*") ||
       (this->getMetadatas().value("ows_enable_request").contains("GetMap")))
      && (! this->getMetadatas().value("ows_enable_request").contains("!GetMap"))));
}

bool MapfileParser::wmsGetLegendGraphicEnabled() {
  return (((this->getMetadatas().value("wms_enable_request").contains("*") ||
       (this->getMetadatas().value("wms_enable_request").contains("GetLegendGraphic")))
      && (! this->getMetadatas().value("wms_enable_request").contains("!GetLegendGraphic")))
    ||
    ((this->getMetadatas().value("ows_enable_request").contains("*") ||
       (this->getMetadatas().value("ows_enable_request").contains("GetLegendGraphic")))
      && (! this->getMetadatas().value("ows_enable_request").contains("!GetLegendGraphic"))));
}

bool MapfileParser::wmsGetCapabilitiesEnabled() {
  return (((this->getMetadatas().value("wms_enable_request").contains("*") ||
       (this->getMetadatas().value("wms_enable_request").contains("GetCapabilities")))
      && (! this->getMetadatas().value("wms_enable_request").contains("!GetCapabilities")))
    ||
    ((this->getMetadatas().value("ows_enable_request").contains("*") ||
       (this->getMetadatas().value("ows_enable_request").contains("GetCapabilities")))
      && (! this->getMetadatas().value("ows_enable_request").contains("!GetCapabilities"))));
}

bool MapfileParser::wmsGetFeatureInfoEnabled() {
  return (((this->getMetadatas().value("wms_enable_request").contains("*") ||
       (this->getMetadatas().value("wms_enable_request").contains("GetFeatureInfo")))
      && (! this->getMetadatas().value("wms_enable_request").contains("!GetFeatureInfo")))
    ||
    ((this->getMetadatas().value("ows_enable_request").contains("*") ||
       (this->getMetadatas().value("ows_enable_request").contains("GetFeatureInfo")))
      && (! this->getMetadatas().value("ows_enable_request").contains("!GetFeatureInfo"))));
}

QString MapfileParser::getMetadataWmsTitle() {
  QString ret = this->getMetadatas().value("wms_title", QString());
  // Empty, lets try with OWS_TITLE
  if (ret.isEmpty()) {
    return this->getMetadatas().value("ows_title", QString());
  }
  return ret;
}

QString MapfileParser::getMetadataWfsTitle() {
  QString ret = this->getMetadatas().value("wfs_title", QString());
  if (ret.isEmpty()) {
    return this->getMetadatas().value("ows_title", QString());
  }
  return ret;
}

QString MapfileParser::getMetadataWmsOnlineresource() {
  QString ret = this->getMetadatas().value("wms_onlineresource", QString());
  if (ret.isEmpty()) {
    return this->getMetadatas().value("ows_onlineresource", QString());
  }
  return ret;
}

QString MapfileParser::getMetadataWfsOnlineresource() {
  QString ret = this->getMetadatas().value("wfs_onlineresource", QString());
  if (ret.isEmpty()) {
    return this->getMetadatas().value("ows_onlineresource", QString());
  }
  return ret;
}

QString MapfileParser::getMetadataWmsSrs() {
  QString ret = this->getMetadatas().value("wms_srs", QString());
  if (ret.isEmpty()) {
    return this->getMetadatas().value("ows_srs", QString());
  }
  return ret;
}

QString MapfileParser::getMetadataWfsSrs() {
  QString ret = this->getMetadatas().value("wfs_srs", QString());
  if (ret.isEmpty()) {
    return this->getMetadatas().value("ows_srs", QString());
  }
  return ret;
}


QString MapfileParser::getShapepath() {
    if (this->map)
        return this->map->shapepath;
    return NULL;
}

void MapfileParser::setShapepath(const QString & shapepath) {
    if (this->map) {
        if (this->map->shapepath) {
            free(this->map->shapepath);
        }
        this->map->shapepath = (char *) strdup(shapepath.toStdString().c_str());
    }
}

QString MapfileParser::getSymbolSet() {
    if (this->map) {
        return this->map->symbolset.filename;
    }
    return QString();
}

void MapfileParser::setSymbolSet(const QString & symbolset) {
    if (this->map) {
        if (this->map->symbolset.filename) {
            free(this->map->symbolset.filename);
        }
        this->map->symbolset.filename = (char *) strdup(symbolset.toStdString().c_str());
    }
}

QString MapfileParser::getFontSet() {
    if (this->map)
        return this->map->fontset.filename;
    return QString();
}

void MapfileParser::setFontSet(const QString & fontset) {
    if (this->map) {
        if (this->map->fontset.filename) {
            free(this->map->fontset.filename);
        }
        this->map->fontset.filename = (char *) strdup(fontset.toStdString().c_str());
    }
}


double MapfileParser::getResolution() {
    if (this->map) {
        return this->map->resolution;
    }
    return -1;
}

void MapfileParser::setResolution(const double & resolution) {
    if (this->map) {
        this->map->resolution = resolution;
    }
}

double MapfileParser::getDefResolution() {
    if (this->map)
        return this->map->defresolution;
    return -1;
}

void MapfileParser::setDefResolution(const double & resolution) {
    if (this->map) {
        this->map->defresolution = resolution;
    }
}

float MapfileParser::getAngle() {
    if (this->map)
        return this->map->gt.rotation_angle;
    return -1.0;
}

void MapfileParser::setAngle(const float & angle) {
    if (this->map) {
        this->map->gt.rotation_angle = angle;
    }
}

QString MapfileParser::getTemplatePattern() {
    if (this->map) {
        return this->map->templatepattern;
    }
    return QString();
}

void MapfileParser::setTemplatePattern(const QString & pattern) {
    if (this->map) {
        if (this->map->templatepattern) {
            free(this->map->templatepattern);
        }
        this->map->templatepattern = (char *) strdup(pattern.toStdString().c_str());
    }
}

QString MapfileParser::getDataPattern() {
    if (this->map) {
        return this->map->datapattern;
    }
    return QString();
}

void MapfileParser::setDataPattern(const QString & pattern) {
    if (this->map) {
        if (this->map->datapattern) {
            free(this->map->datapattern);
        }
        this->map->datapattern = (char *) strdup(pattern.toStdString().c_str());
    }
}


QString const & MapfileParser::getMapfilePath() const {
  return this->mapPath;
}

QString const & MapfileParser::getMapfileName() const {
  return this->filename;
}

QColor MapfileParser::getImageColor() const {
   if (this->map) {
     colorObj colorObj = this->map->imagecolor;
     return QColor(colorObj.red, colorObj.green, colorObj.blue, colorObj.alpha);
   }
   return QColor();
}

void MapfileParser::setImageColor(QColor const & color) {
  if (this->map) {
    this->map->imagecolor.red = color.red();
    this->map->imagecolor.green = color.green();
    this->map->imagecolor.blue = color.blue();
    this->map->imagecolor.alpha = color.alpha();
  }
}

/**
 * Gets the known output format from the Mapfile.
 */
QList<OutputFormat *> const & MapfileParser::getOutputFormats() const {
  return this->outputFormats;
}


/**
 * Does basically the same as the previous method, but for a given
 * name. This implies that the OF exists in the mapfile.
 *
 * Used by the QUndo commands, to get the state of an ouptut format
 * just before it goes modified.
 *
 * This is the role of the caller to destroy the created object.
 * TODO: scan this->outputFormats instead ?
 * Then remove the delete() from the QUndo command ?
 */
OutputFormat * MapfileParser::getOutputFormat(QString const & name) {

  if (! this->map)
    return NULL;

  int ofIdx = msGetOutputFormatIndex(this->map, name.toStdString().c_str());
  outputFormatObj * msOf = this->map->outputformatlist[ofIdx];
  if (msOf == NULL)
    return NULL;

  OutputFormat * item = new OutputFormat(msOf->name,
                                         msOf->mimetype,
                                         msOf->driver,
                                         msOf->extension,
                                         msOf->imagemode,
                                         msOf->transparent,
                                         OutputFormat::UNCHANGED);

  for (int j = 0 ; j < msOf->numformatoptions; ++j) {
    QStringList kv = QString(msOf->formatoptions[j]).split("=");
    if (kv.size() == 2)
      item->addFormatOption(kv[0], kv[1]);
  }
  return item;
}

void MapfileParser::removeOutputFormat(OutputFormat const * of) {
  if (! this->map)
    return;

  for (int i = 0; i < this->outputFormats.size(); ++i) {
    if (this->outputFormats[i]->getName() == of->getName()) {
      delete this->outputFormats[i];
      this->outputFormats.removeAt(i);
      break;
    }
  }
  msRemoveOutputFormat(this->map, of->getName().toStdString().c_str());
}

void MapfileParser::updateOutputFormat(OutputFormat * const of) {
  if (! this->map)
    return;

  for (int i = 0; i < this->outputFormats.size(); ++i) {
    if (this->outputFormats[i]->getName() == of->getName()) {
      delete this->outputFormats[i];
      this->outputFormats[i] = of;
      break;
    }
  }

  int ofIdx = msGetOutputFormatIndex(this->map, of->getOriginalName().toStdString().c_str());
  outputFormatObj * msOf = this->map->outputformatlist[ofIdx];

  // name
  if (of->getName() != msOf->name) {
    free(msOf->name);
    msOf->name = strdup(of->getName().toStdString().c_str());
  }
  // mimetype
  if (of->getMimeType() != msOf->mimetype) {
    free(msOf->mimetype);
    msOf->mimetype = strdup(of->getMimeType().toStdString().c_str());
  }
  // driver
  QString fullyQualifiedDriver = of->getDriver();
  if ((of->getDriver() == "GDAL") || (of->getDriver() == "OGR")) {
    fullyQualifiedDriver += "/" + of->getGdalDriver();
  }
  if (fullyQualifiedDriver != msOf->driver) {
   free(msOf->driver);
   msOf->driver = strdup(fullyQualifiedDriver.toStdString().c_str());
  }
  // extension
  if (of->getExtension() != msOf->extension) {
    free(msOf->extension);
    msOf->extension = strdup(of->getExtension().toStdString().c_str());
  }
  // imagemode
  if (of->getImageMode() != msOf->imagemode) {
    msOf->imagemode  = of->getImageMode();
  }
  // transparent
  if (of->getTransparent() != msOf->transparent) {
    msOf->transparent = of->getTransparent();
  }

  // format options

  // first, removes the format options
  for (int i = 0; i < msOf->numformatoptions; ++i) {
    free(msOf->formatoptions[i]);
    msOf->formatoptions[i] = NULL;
  }
  msOf->numformatoptions = 0;
  // then, recreates them
  QHash<QString,QString> fmtOpts = of->getFormatOptions();
  QStringList fmtKeys = fmtOpts.keys();
  for (int i = 0 ; i < fmtKeys.size(); ++i) {
    msSetOutputFormatOption(msOf, fmtKeys[i].toStdString().c_str(),
                            fmtOpts[fmtKeys[i]].toStdString().c_str());
  }

}

void MapfileParser::addOutputFormat(OutputFormat * const of) {
  if (!this->map)
    return;

  this->outputFormats  << of;
  QString fullyQualifiedDriver = of->getDriver();
  if ((of->getDriver() == "GDAL") || (of->getDriver() == "OGR")) {
    fullyQualifiedDriver += "/" + of->getGdalDriver();
  }

  outputFormatObj * newMsOf = msCreateDefaultOutputFormat(this->map,
                                                          fullyQualifiedDriver.toStdString().c_str(),
                                                          of->getName().toStdString().c_str());

  if (! newMsOf) {
    qDebug() << "newly created output format is NULL, should not happen (mapserver side)";
    return;
  }
  newMsOf->mimetype  = strdup(of->getMimeType().toStdString().c_str());
  newMsOf->extension = strdup(of->getExtension().toStdString().c_str());
  newMsOf->imagemode = of->getImageMode();
  newMsOf->transparent = of->getTransparent();
  // add format options
  QHash<QString,QString> fmtOpts = of->getFormatOptions();
  QStringList fmtKeys = fmtOpts.keys();
  for (int i = 0 ; i < fmtKeys.size(); ++i) {

    msSetOutputFormatOption(newMsOf,
                            fmtKeys[i].toStdString().c_str(),
                            fmtOpts[fmtKeys[i]].toStdString().c_str());
  }

  // No idea why, but when calling msCreateDefaultOutputFormat(), the
  // created output format is considered "outside" of the mapfile.
  newMsOf->inmapfile = MS_TRUE;
}

QString const & MapfileParser::getDefaultOutputFormat() const {
  return this->defaultOutputFormat;
}

void MapfileParser::setDefaultOutputFormat(QString const & of) {
  if (! this->map)
    return;
  if (this->map->imagetype) {
    free(this->map->imagetype);
  }
  this->defaultOutputFormat = of;
  this->map->imagetype = strdup(of.toStdString().c_str());
}

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

MapfileParser::~MapfileParser() {
  if (this->map) {
    msFreeMap(this->map);
  }
  if (this->currentImageBuffer) {
    free(this->currentImageBuffer);
  }
  for (int i = 0; i < this->outputFormats.size(); ++i)
    delete this->outputFormats[i];
}

/**
 * Static variables
 */

QStringList MapfileParser::units = QStringList() << "inches" << "feet" << "miles" << "meters" << "kilometers" <<
            "dd" << "pixels" << "percentages" << "nauticalmiles";

QStringList MapfileParser::imageTypes = QStringList() << "jpeg" << "pdf" << "png" << "svg";

QStringList MapfileParser::missingData = QStringList() << "" << "FAIL" << "LOG" << "IGNORE";

QStringList MapfileParser::ogcMapOptions = QStringList() << "" << "ows_http_max_age" << "ows_schemas_location" <<
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

QStringList MapfileParser::drivers = QStringList() << "AGG/PNG" <<
                   "AGG/JPEG" <<  "GD/GIF" << "GD/PNG" << "TEMPLATE" << "GDAL"<< "OGR";

QStringList MapfileParser::defaultImageModes = QStringList() << "" << "RGB" << "RGBA" ;
QStringList MapfileParser::IMgdGifgdPng      = QStringList() << MapfileParser::defaultImageModes << "PC256";
QStringList MapfileParser::IMTemplateOgr     = QStringList() << MapfileParser::defaultImageModes << "FEATURE";
QStringList MapfileParser::IMGdal            = QStringList() << MapfileParser::defaultImageModes << "BYTE" << "INT16" << "FLOAT32";
// QHash<QString, QStringList> MapfileParser::ImageMode
static QHash<QString,QStringList> initImageModes() {
  QHash<QString,QStringList> ret = QHash<QString,QStringList>();
    // * PC256 only for GD/GIF and GD/PNG
    // * RGB
    // * RGBA
    // * BYTE only for RASTER GDAL and WMS
    // * INT16 only for RASTER GDAL and WMS
    // * FLOAT32 only for RASTER GDAL and WMS
    // * FEATURE only via OGR and TEMPLATE
  ret.insert("AGG/PNG", MapfileParser::defaultImageModes);
  ret.insert("AGG/JPEG", MapfileParser::defaultImageModes);
  ret.insert("GD/GIF", MapfileParser::IMgdGifgdPng);
  ret.insert("GD/PNG", MapfileParser::IMgdGifgdPng);
  ret.insert("TEMPLATE", MapfileParser::IMTemplateOgr);
  ret.insert("GDAL", MapfileParser::IMGdal);
  ret.insert("OGR", MapfileParser::IMTemplateOgr);
  return ret;
}
QHash<QString, QStringList> MapfileParser::imageModes = initImageModes();

