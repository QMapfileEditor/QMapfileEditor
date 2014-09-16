#include "mapfileparser.h"

#include <stdlib.h>


App::MapfileParser::MapfileParser(const std::string filename)
{
    this->map = umnms_new_map((char *) filename.c_str());
    std::cout << "mapfile loaded.";

}

App::MapfileParser::~MapfileParser(void) {
    free(map);
};
