#include "testmapfileparser.h"
#include "../mapfileparser.h"

QTEST_MAIN(TestMapfileParser)

void TestMapfileParser::initMapfileParser()
{
  MapfileParser * p = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->isLoaded());
  QVERIFY(p->getMapName() == "World Map");
  delete p;

  // test with an invalid path
  p = new MapfileParser("/dev/urandom");
  QVERIFY(! p->isLoaded());
  delete p;
}

/** test getters / setters for the map name */
void TestMapfileParser::testMapName() {
  MapfileParser * p = new MapfileParser();

  QVERIFY(p->getMapName() == "MS");

  p->setMapName("test setMapName()");
  QVERIFY(p->getMapName() == "test setMapName()");

  delete p;
}


/** test getters / setters for the map filepath */
void TestMapfileParser::testFilePath() {
  MapfileParser * p = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->getMapfilePath().size() > 0);
  delete p;

  p = new MapfileParser();
  QVERIFY(p->getMapfilePath().isEmpty());
  delete p;

}


