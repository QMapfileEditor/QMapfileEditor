#ifndef MAPFILEPARSER_H
#define MAPFILEPARSER_H

#include <mapserver.h>
#include <mapfile.h>
#include <string>
#include <iostream>

namespace App {
  class MapfileParser;
}

class MapfileParser
{
  public:
      MapfileParser(const std::string filename);
      ~MapfileParser();
  private:
      mapObj * map;

};

#endif // MAPFILEPARSER_H
