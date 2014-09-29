#include "testmapfileparser.h"
#include "../mapfileparser.h"

QTEST_MAIN(TestMapfileParser)

void TestMapfileParser::initMapfileParser()
{
  MapfileParser * p = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->isLoaded());
  QVERIFY(p->getMapName() == "World Map");
  delete p;

}
