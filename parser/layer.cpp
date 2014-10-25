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
