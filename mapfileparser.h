#ifndef MAPFILEPARSER_H
#define MAPFILEPARSER_H

#include <string>
#include <iostream>



class MapfileParser
{
  public:
      MapfileParser(const std::string filename);
      ~MapfileParser();
  private:
      struct mapObj * map;

};

#endif // MAPFILEPARSER_H
