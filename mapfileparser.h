#ifndef MAPFILEPARSER_H
#define MAPFILEPARSER_H

#include <string>
#include <iostream>

#include "libmapserver-bridge.h"


namespace App {

class MapfileParser
{
  public:
      MapfileParser(const std::string filename);
      ~MapfileParser();
  private:
      mapObj * map;

};

}
#endif // MAPFILEPARSER_H
