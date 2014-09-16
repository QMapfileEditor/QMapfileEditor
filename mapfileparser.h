#ifndef MAPFILEPARSER_H
#define MAPFILEPARSER_H

#include <QVector>
#include <QString>

#include <string>
#include <iostream>



class MapfileParser
{
  public:
      MapfileParser(const std::string filename);
      ~MapfileParser();
      QVector<QString> * getLayers(void);
  private:
      struct mapObj * map = NULL;
      QVector<QString> * layers = NULL;


};

#endif // MAPFILEPARSER_H
