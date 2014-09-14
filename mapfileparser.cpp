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
    std::cout << "mapfile loaded.";

}

MapfileParser::~MapfileParser() {
    free(map);
};
