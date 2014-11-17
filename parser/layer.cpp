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

Layer::Layer(QString const & name, struct mapObj * map):
name(name), map(map) {

  layerObj * l = getInternalLayerObj();
  if (l) {
    this->status = l->status;
    this->type = Layer::layerType.at(l->type);
    // TODO geomType ?
    this->opacity = l->opacity;
    this->mask = l->mask; // TODO if mask == NULL ??
    this->group = l->group;
    this->requires = l->requires;
    this->plugin = l->plugin_library; // TODO ?
    //TODO projection ? map->project and map->projection refers to != objects
    this->minx = l->extent.minx;
    this->miny = l->extent.miny;
    this->maxx = l->extent.maxx;
    this->maxy = l->extent.maxy;
    // TODO layer->filter is a complex object
    this->minScale = l->minscaledenom;
    this->maxScale = l->maxscaledenom;
    // TODO geomTransform  vs l->transform ?
    this->tolerance = l->tolerance;
    // TODO toleranceUnits vs l->toleranceunits ?
    // TODO processing (QString) vs l->processing (char ** / QStringList)
    this->transform = l->transform;
    this->maxfeatures = l->maxfeatures;
    this->mingeowidth = l->mingeowidth;
    this->maxgeowidth = l->maxgeowidth;
    // TODO layerTemplate ?
    this->header = l->header;
    this->footer = l->footer;
    this->labelItem = l->labelitem;
    this->maxScaleDenomLabel = l->labelmaxscaledenom;
    this->minScaleDenomLabel = l->labelminscaledenom;
    // TODO labelAngleItem (not found in layerObj)
    this->labelCache = l->labelcache;
    this->postLabelCache = l->postlabelcache;
    // TODO labelSizeItem ? (not found in layerObj)
    this->labelRequires = l->labelrequires;
    // TODO l->validation is a complex structure (vs QString)
    this->debugLevel = l->debug;

  }

}

QString const & Layer::getName() {
  return name;
}

void Layer::setName(QString const & newName) {
  // TODO: check if name not already taken
  layerObj * l = getInternalLayerObj();
  if (! l)
    return;
  if (l->name) {
    free(l->name);
  }
  l->name  = strdup(newName.toStdString().c_str());
  name = newName;
}


/* private methods */

int Layer::getInternalIndex(void) {
  if (! this->map) {
    return -1;
  }
  return msGetLayerIndex(this->map, (char *) name.toStdString().c_str());
}

layerObj * Layer::getInternalLayerObj(void) {
  int idx = getInternalIndex();

  if ((idx < 0) || (idx > this->map->numlayers))
   return NULL;

 return GET_LAYER(this->map, idx);

}


QStringList Layer::layerType = QStringList() << "MS_LAYER_POINT" << "MS_LAYER_LINE" << "MS_LAYER_POLYGON"
                                                    << "MS_LAYER_RASTER" << "MS_LAYER_ANNOTATION" << "MS_LAYER_QUERY"
                                                    << "MS_LAYER_CIRCLE" << "MS_LAYER_TILEINDEX"<< "MS_LAYER_CHART";
