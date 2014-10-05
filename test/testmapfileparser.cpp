#include "testmapfileparser.h"
#include "../mapfileparser.h"

QTEST_MAIN(TestMapfileParser)

/** tests construtor */
void TestMapfileParser::testInitMapfileParser()
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

/** tests getters / setters for the map name */
void TestMapfileParser::testMapName() {
  MapfileParser * p = new MapfileParser();

  QVERIFY(p->getMapName() == "MS");

  p->setMapName("test setMapName()");
  QVERIFY(p->getMapName() == "test setMapName()");

  delete p;
}


/** tests getters / setters for the map filepath */
void TestMapfileParser::testFilePath() {
  MapfileParser * p = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->getMapfilePath().size() > 0);
  delete p;

  p = new MapfileParser();
  QVERIFY(p->getMapfilePath().isEmpty());
  delete p;

}

/** tests drawing map (MapfileParser::getCurrentMapImage() */
void TestMapfileParser::testGetCurrentMapImage() {
  MapfileParser * p = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->isLoaded());

  unsigned char * buffer = p->getCurrentMapImage(0,0);
  QVERIFY(buffer != NULL);

  QVERIFY(p->getCurrentMapImageSize() > 0);
  /** Should be PNG */
  QVERIFY(buffer[0] == 0x89 && buffer[1] == 0x50 && buffer[2] == 0x4e);

  // same with a given size
  buffer = p->getCurrentMapImage(500,500);
  QVERIFY(buffer != NULL);
  QVERIFY(p->getCurrentMapImageSize() > 0);
  QVERIFY(buffer[0] == 0x89 && buffer[1] == 0x50 && buffer[2] == 0x4e);
  QPixmap *px = new QPixmap();
  px->loadFromData(buffer, p->getCurrentMapImageSize());
  // Makes sure the generated PNG is 500x500
  QVERIFY(px->width() == 500 && px->height() == 500);

  if (px) delete px;
  if (p) delete p;

}

/** tests layers getters / setters */
void TestMapfileParser::testLayers() {
  MapfileParser * p = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->isLoaded());

  QStringList layers = p->getLayers();
  QVERIFY(layers.size() == 2);
  QVERIFY(layers.at(0) == "world_raster");
  QVERIFY(layers.at(1) == "world_adm0");

  if (p) delete p;

  p = new MapfileParser();

  // on a blank mapfile, no layer
  QVERIFY(p->getLayers().size() == 0);

  if (p) delete p;
}
