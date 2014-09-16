#ifndef MAPFILEPARSER_H
#define MAPFILEPARSER_H



class MapfileParser
{
  public:
      MapfileParser(const std::string filename);
      ~MapfileParser();
  private:
      struct mapObj * map;

};

#endif // MAPFILEPARSER_H
