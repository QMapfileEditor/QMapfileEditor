#include <mapserver.h>
#include <mapfile.h>


#include "libmapserver-bridge.h"

/** 
 *
 * This file is meant to provide a bridge between raw-C code from libmapserver
 * and C++ from the Qt project, waiting for a proper API to be defined
 * mapserver-side.
 *
 */

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

