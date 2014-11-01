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

  if (idx < 0)
   return NULL;
 return GET_LAYER(this->map, idx);

}
