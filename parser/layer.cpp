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

#include "mapserver.h"

#include "layer.h"
#include "mapfileparser.h"

#include <QDebug>

Layer::Layer(QString const & name, struct mapObj * map):
map(map) {
  this->name = name;
}

QString const & Layer::getName() const {
  return name;
}

void Layer::setName(QString const & newName) {
  layerObj * l = getInternalLayerObj();
  if (! l)
    return;
  if (l->name) {
    free(l->name);
  }
  l->name  = strdup(newName.toStdString().c_str());
  name = newName;
}

double Layer::getMaxScaleDenomLabel() const {
  layerObj * l = getInternalLayerObj();
  if (! l)
    return -1.0;
  return l->labelmaxscaledenom;
}



double Layer::getMinScaleDenomLabel() const {
  layerObj * l = getInternalLayerObj();
  if (! l)
    return -1.0;
  return l->labelminscaledenom;
}

int Layer::getStatus() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->status;
  return -1;
}

void Layer::setStatus(int const newStatus) {
  if (newStatus < 0 || newStatus > 2)
    return;
  layerObj * l = getInternalLayerObj();
  if (l)
    l->status = newStatus;
}

QString Layer::getRequires() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->requires;
  return QString();
}

void Layer::setRequires(QString const & newRequires) {
  layerObj * l = getInternalLayerObj();
  if (l) {
    if (l->requires) {
      free(l->requires);
      l->requires = NULL;
    }
    if (newRequires.isEmpty()) {
      return;
    }
    l->requires = strdup(newRequires.toStdString().c_str());
  }
  return;
}

QString Layer::getGroup() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->group;
  return QString();
}

void Layer::setGroup(QString const &newGroup) {
  layerObj * l = getInternalLayerObj();
  if (l) {
    if (l->group) {
      free(l->group);
      l->group = NULL;
    }
    if (newGroup.isEmpty())
      return;
    l->group = strdup(newGroup.toStdString().c_str());
  }
  return;
}

QString Layer::getType() const  {
  layerObj * l = getInternalLayerObj();
  if (l)
    return layerType.at(l->type);
  return QString();
}

int Layer::getOpacity() const {
  layerObj * l = getInternalLayerObj();
#if MS_VERSION_MAJOR < 7
  if (l)
    return l->opacity;
#else
  if (l && l->compositer)
    return l->compositer->opacity;
#endif
  return -1;
}

void Layer::setOpacity(int const newOpacity) {
  layerObj * l = getInternalLayerObj();
  if (l) {
    // No need for prepocessor magick here,
    // mapserver internals will do the job for us
    msSetLayerOpacity(l, newOpacity);
  }
  return;
}

QString Layer::getMask() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->mask;
  return QString();
}

void Layer::setMask(QString const &newMask) {
  layerObj * l = getInternalLayerObj();
  if (l) {
    if (l->mask) {
      free(l->mask);
      l->mask = NULL;
    }
    if (newMask.isEmpty()) {
      return;
    }
    l->mask = strdup(newMask.toStdString().c_str());
  }
  return;
}

QString Layer::getUnits() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return MapfileParser::units.at(l->units);
  return QString();
}

QString Layer::getSizeUnits() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return MapfileParser::units.at(l->sizeunits);
  return QString();
}

double Layer::getMinX() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->extent.minx;
  return -1;
}

double Layer::getMinY() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->extent.miny;
  return -1;
}

double Layer::getMaxX() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->extent.maxx;
  return -1;
}

double Layer::getMaxY() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->extent.maxy;
  return -1;
}

double Layer::getMinScaleDenom() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->minscaledenom;
  return -1.0;
}
void Layer::setMinScaleDenom(double const newMin) {
  layerObj * l = getInternalLayerObj();
  if (! l)
    return;
  l->minscaledenom = newMin;
}

double Layer::getMaxScaleDenom() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->maxscaledenom;
  return -1.0;
}

void Layer::setMaxScaleDenom(double const newMax) {
  layerObj * l = getInternalLayerObj();
  if (! l)
    return;
  l->maxscaledenom = newMax;
}

QString Layer::getPlugin() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->plugin_library;
  return QString();
}

double Layer::getTolerance() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->tolerance;
  return -1.0;
}

QString Layer::getToleranceUnits() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    // TODO toleranceunits is an int (mapserver.h around line 1598)
    return QString();
  return QString();
}

int Layer::getMaxFeatures() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->maxfeatures;
  return -1;
}

double Layer::getMinGeoWidth() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->mingeowidth;
  return -1.0;
}

double Layer::getMaxGeoWidth() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->maxgeowidth;
  return -1.0;
}

QString Layer::getClassGroup() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->classgroup;
  return QString();
}

QString Layer::getTemplate() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->_template;
  return QString();
}

QString Layer::getHeader() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->header;
  return QString();
}

QString Layer::getFooter() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->footer;
  return QString();
}

QString Layer::getStyleItem() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->styleitem;
  return QString();
}

QString Layer::getFilterItem() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->filteritem;
  return QString();
}

QString Layer::getLabelItem() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->labelitem;
  return QString();
}

QString Layer::getClassItem() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->classitem;
  return QString();
}

double Layer::getSymbolScaleDenom() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->symbolscaledenom;
  return -1.0;
}

bool Layer::getLabelCache() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->labelcache;
  return false;
}

bool Layer::getPostLabelCache() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->postlabelcache;
  return false;
}

int Layer::getDebugLevel() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->debug;
  return -1;
}

void Layer::setDebugLevel(int const newLevel) {
  layerObj * l = getInternalLayerObj();
  if (l) {
    l->debug = newLevel;
  }
  return;
}

QString Layer::getLabelRequires() const {
  layerObj * l = getInternalLayerObj();
  if (l)
    return l->labelrequires;
  return QString();
}


/* private methods */

int Layer::getInternalIndex(void) const {
  if ((! this->map) || name.isNull() || name.isEmpty()) {
    return -1;
  }

  return msGetLayerIndex(this->map, (char *) name.toStdString().c_str());
}

layerObj * Layer::getInternalLayerObj(void) const {
  int idx = getInternalIndex();

  if ((idx < 0) || (idx > this->map->numlayers))
   return NULL;

 return GET_LAYER(this->map, idx);

}

Layer::~Layer() {
 qDebug() << "Destroying Layer " << this->name ;
}

QStringList Layer::layerType = QStringList() << "MS_LAYER_POINT" << "MS_LAYER_LINE" << "MS_LAYER_POLYGON"
                                                    << "MS_LAYER_RASTER" << "MS_LAYER_ANNOTATION" << "MS_LAYER_QUERY"
                                                    << "MS_LAYER_CIRCLE" << "MS_LAYER_TILEINDEX"<< "MS_LAYER_CHART";



// Methods related to the Qt representation of the layers

LayerModel::LayerModel(QObject * parent, QList<Layer *> const & l) : QAbstractListModel(parent), layers(l) {}


LayerModel::~LayerModel() {}

void LayerModel::setLayers(QList<Layer *> const  & layers) {
  beginResetModel();
  this->layers = layers;
  endResetModel();
}

Layer * LayerModel::getLayer(const QModelIndex &m) const {
  if ((m.row() < 0) || m.row() > layers.size())
    return NULL;
  return layers.at(m.row());
}

void LayerModel::removeLayer(const QModelIndex &m) {
   if (m.row() < 0 || m.row() > layers.size())
    return;

  beginResetModel();

  Layer * toBeRemoved = layers.takeAt(m.row());
  // TODO: We might need the same mechanism as on OutputFormats
  // (i.e. state - ADDED ...)
  //if ((toBeRemoved->getState() == OutputFormat::ADDED)
  //    ||(toBeRemoved->getState() == OutputFormat::ADDED_SAVED)) {
  //  delete toBeRemoved;
  //} else {
  removedLayers.append(toBeRemoved);
  //}
  endResetModel();

}

int LayerModel::rowCount(const QModelIndex & parent) const {
  Q_UNUSED(parent);
  return layers.size();
}

int LayerModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return LayerModel::LAYER_DEBUG_LEVEL + 1;
}

QVariant LayerModel::data(const QModelIndex &index, int role) const {
  if ((role != Qt::DisplayRole) && (role != Qt::EditRole))
    return QVariant();
  if ((index.row() < 0) || (index.row() > layers.size()))
    return QVariant();

  Layer * l = layers.at(index.row());

  if (l == NULL)
    return QVariant();

  switch (index.column()) {
    case LayerModel::LAYER_NAME:
      return QVariant(l->getName());
    case LayerModel::LAYER_STATUS:
      return QVariant(l->getStatus());
    case LayerModel::LAYER_TYPE:
      return QVariant(l->getType());
    case LayerModel::LAYER_OPACITY:
      return QVariant(l->getOpacity());
    case LayerModel::LAYER_MASK:
      return QVariant(l->getMask());
    case LayerModel::LAYER_MIN_X:
      return QVariant(l->getMinX());
    case LayerModel::LAYER_MAX_X:
      return QVariant(l->getMaxX());
    case LayerModel::LAYER_MIN_Y:
      return QVariant(l->getMinY());
    case LayerModel::LAYER_MAX_Y:
      return QVariant(l->getMaxY());
    case LayerModel::LAYER_MIN_SCALE:
      return QVariant(l->getMinScaleDenom());
    case LayerModel::LAYER_MAX_SCALE:
      return QVariant(l->getMaxScaleDenom());
    case LayerModel::LAYER_TOLERANCE:
      return QVariant(l->getTolerance());
    case LayerModel::LAYER_MAX_FEATURES:
      return QVariant(l->getMaxFeatures());
    case LayerModel::LAYER_MIN_GEO_WIDTH:
      return QVariant(l->getMinGeoWidth());
    case LayerModel::LAYER_MAX_GEO_WIDTH:
      return QVariant(l->getMaxGeoWidth());
    case LayerModel::LAYER_HEADER:
      return QVariant(l->getHeader());
    case LayerModel::LAYER_FOOTER:
      return QVariant(l->getFooter());
    case LayerModel::LAYER_LABEL_ITEM:
      return QVariant(l->getLabelItem());
    case LayerModel::LAYER_SYMBOL_SCALE_DENOM:
      return QVariant(l->getSymbolScaleDenom());
    case LayerModel::LAYER_MAX_SCALE_DENOM_LABEL:
      return QVariant(l->getMaxScaleDenomLabel());
    case LayerModel::LAYER_MIN_SCALE_DENOM_LABEL:
      return QVariant(l->getMinScaleDenomLabel());
    case LayerModel::LAYER_LABEL_CACHE:
      return QVariant(l->getLabelCache());
    case LayerModel::LAYER_POST_LABEL_CACHE:
      return QVariant(l->getPostLabelCache());
    case LayerModel::LAYER_LABEL_REQUIRES:
      return QVariant(l->getLabelRequires());
    case LayerModel::LAYER_DEBUG_LEVEL:
      return QVariant(l->getDebugLevel());
    default:
      return QVariant();
  }
}


QVariant LayerModel::headerData (int section, Qt::Orientation orientation, int role) const {
  Q_UNUSED(section);
  Q_UNUSED(orientation);
  if (role != Qt::DisplayRole)
    return QVariant();
  return QVariant(QObject::tr("Layers"));
}


