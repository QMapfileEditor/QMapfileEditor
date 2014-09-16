
#include <mapserver.h>
#include <mapfile.h>

#include <string>
#include <iostream>

#include "mapfileparser.h"

// Defining missing functions
// declared in mapserver-api
extern "C" {
mapObj * umnms_new_map(char *filename) {
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


MapfileParser::MapfileParser(const std::string filename)
{
    this->map = umnms_new_map((char *) filename.c_str());
    this->layers = new QVector<QString *>();

    // Loads layers into a map
    for (int i = 0; i <  this->map->numlayers ; i++) {
      QString *curStr = new QString(this->map->layers[i]->name);
      this->layers->append(curStr);
    }

}

QVector<QString *> * MapfileParser::getLayers() {
  return this->layers;

}

MapfileParser::~MapfileParser() {
    free(map);
};
