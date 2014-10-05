#include "testmapfileparser.h"
#include "../mapfileparser.h"

#include <QDir>

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
  QImage *im = new QImage();
  im->loadFromData(buffer, p->getCurrentMapImageSize());
  // Makes sure the generated PNG is 500x500
  QVERIFY(im->width() == 500 && im->height() == 500);

  if (im) delete im;
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

/** test map status */
void TestMapfileParser::testStatus() {
  MapfileParser * p  = new MapfileParser();

  // initial state: Map status is true
  QVERIFY(p->getMapStatus());

  p->setMapStatus(false);

  QVERIFY(! p->getMapStatus());

  if (p) delete p;

}
/** test map width / height */
void TestMapfileParser::testWidthHeight() {
  MapfileParser * p  = new MapfileParser();
  QVERIFY(p->getMapWidth() == -1);
  QVERIFY(p->getMapHeight() == -1);

  p->setMapSize(500,500);

  QVERIFY(p->getMapWidth() == 500);
  QVERIFY(p->getMapHeight() == 500);

  if (p) delete p;
}

/** test max size */
void TestMapfileParser::testMapMaxSize() {

  MapfileParser * p  = new MapfileParser();

  QVERIFY(p->getMapMaxsize() == 2048);

  p->setMapMaxsize(1024);

  QVERIFY(p->getMapMaxsize() == 1024);

  if (p) delete p;
}

/** test map units */
void TestMapfileParser::testMapUnits() {

  MapfileParser * p  = new MapfileParser();

  QVERIFY(MapfileParser::units.at(p->getMapUnits()) == "meters");

  p->setMapUnits("nauticalmiles");

  QVERIFY(MapfileParser::units.at(p->getMapUnits()) == "nauticalmiles");

  if (p) delete p;
}

/** test imagetype */
void TestMapfileParser::testImageType() {

  MapfileParser * p  = new MapfileParser("../data/world_mapfile.map");

  QVERIFY(p->getMapImageType() == "png24");

  p->setMapImageType("jpeg");

  QVERIFY(p->getMapImageType() == "jpeg");

  if (p) delete p;
}

// TODO test map projections

/** test map extent */
void TestMapfileParser::testMapExtent() {

  MapfileParser * p  = new MapfileParser();

  // initial values are -1, -1, -1, -1

  QVERIFY(p->getMapExtentMinX() == -1);
  QVERIFY(p->getMapExtentMinY() == -1);
  QVERIFY(p->getMapExtentMaxX() == -1);
  QVERIFY(p->getMapExtentMaxY() == -1);

  p->setMapExtent(-180, -90, 180, 90);

  QVERIFY(p->getMapExtentMinX() == -180);
  QVERIFY(p->getMapExtentMinY() == -90);
  QVERIFY(p->getMapExtentMaxX() == 180);
  QVERIFY(p->getMapExtentMaxY() == 90);

  if (p) delete p;

}

/** test debug
 *  (6 debug levels, stored as integer, from 0 to 5)
 */
void TestMapfileParser::testDebug() {
  MapfileParser * p  = new MapfileParser();
  // default is 0
  QVERIFY(p->getDebug() == 0);

  p->setDebug(5);

  QVERIFY(p->getDebug() == 5);

  if (p) delete p;
}

/** test configuration options
 */
void TestMapfileParser::testConfigOptions() {
  MapfileParser * p  = new MapfileParser("../data/world_mapfile.map");

  QHash<QString,QString> l = p->getConfigOptions();

  QVERIFY(l.size() == 6);

  QVERIFY(p->getDebugFile()           == l.value("MS_ERRORFILE"));
  QVERIFY(p->getConfigMissingData()   == l.value("ON_MISSING_DATA"));
  QVERIFY(p->getConfigContextUrl()    == l.value("CGI_CONTEXT_URL"));
  QVERIFY(p->getConfigEncryptionKey() == l.value("MS_ENCRYPTION_KEY"));
  QVERIFY(p->getConfigNonsquare()     == l.value("MS_NONSQUARE"));
  QVERIFY(p->getConfigProjLib()       == l.value("PROJ_LIB"));

  QVERIFY(p->getConfigOptions().value("PROJ_LIB") == "/usr/local/share/proj/");
  p->setConfigOption("PROJ_LIB", "/usr/share/proj/");
  // size should not have changed
  QVERIFY(p->getConfigOptions().size() == 6);
  // but there should be a new value for PROJ_LIB
  QVERIFY(p->getConfigProjLib() == "/usr/share/proj/");

  p->setConfigOption("MY_CUSTOM_PROPERTY", "a random value");
  QVERIFY(p->getConfigOptions().size() == 7);
  QVERIFY(p->getConfigOptions().value("MY_CUSTOM_PROPERTY") == "a random value");

  p->removeConfigOption("MY_CUSTOM_PROPERTY");
  QVERIFY(p->getConfigOptions().size() == 6);

  if (p) delete p;
}

/** test metadata related methods
 *
 */
void TestMapfileParser::testMetadata() {
  MapfileParser * p  = new MapfileParser("../data/world_mapfile.map");

  QVERIFY(p->getMetadatas().size() == 8);


  QVERIFY(p->getMetadataWmsTitle() == "World Map");
  QVERIFY(p->getMetadataWfsTitle() == "Test OWS");  // the ows one
  QVERIFY(p->getMetadataWmsOnlineresource() == "http://localhost/cgi-bin/mapserv.exe?map=wms.map&");
  QVERIFY(p->getMetadataWfsOnlineresource().isEmpty()); // no ows_onlineresource defined in the sample mapfile
  QVERIFY(p->getMetadataWmsSrs() == "EPSG:4326");
  QVERIFY(p->getMetadataWfsSrs().isEmpty());

  // Testing insertion
  p->setMetadata("ows_srs", "EPSG:2154");

  QVERIFY(p->getMetadatas().size() == 9);
  QVERIFY(p->getMetadataWfsSrs() == "EPSG:2154");

  // testing update
  p->setMetadata("wms_srs", "EPSG:3857");
  QVERIFY(p->getMetadataWmsSrs() == "EPSG:3857");
  QVERIFY(p->getMetadatas().size() == 9);

  // testing removal
  p->removeMetadata("ows_srs");
  QVERIFY(p->getMetadatas().size() == 8);
  QVERIFY(p->getMetadataWfsSrs().isEmpty());

  if (p) delete p;
}

/** test shape path */
void TestMapfileParser::testShapePath() {
  MapfileParser * p  = new MapfileParser("../data/world_mapfile.map");

  QVERIFY(p->getShapepath() == "./");

  p->setShapepath("/usr/share/mapserver/shapepath/");

  QVERIFY(p->getShapepath() == "/usr/share/mapserver/shapepath/");

  if (p) delete p;
}

/** test symbol set */
void TestMapfileParser::testSymbolSet() {
  MapfileParser * p  = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->getSymbolSet() == "symbol.sym");
  p->setSymbolSet("/usr/share/mapserver/symbol.sym");
  QVERIFY(p->getSymbolSet() == "/usr/share/mapserver/symbol.sym");
  if (p) delete p;
}

/** test font set */
void TestMapfileParser::testFontSet() {
  MapfileParser * p  = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->getFontSet() == "test.font");
  p->setFontSet("/usr/share/mapserver/default.font");
  QVERIFY(p->getFontSet() == "/usr/share/mapserver/default.font");
  if (p) delete p;
}

/** test resolution */
void TestMapfileParser::testResolution() {
  MapfileParser * p  = new MapfileParser();
  // default is 72
  QVERIFY(p->getResolution() == 72);

  p->setResolution(48);

  QVERIFY(p->getResolution() == 48);

  if (p) delete p;
}

/** test defresolution */
void TestMapfileParser::testDefResolution() {
  MapfileParser * p  = new MapfileParser();
  QVERIFY(p->getDefResolution() == 72);

  p->setDefResolution(48);

  QVERIFY(p->getDefResolution() == 48);

  if (p) delete p;
}

/** test get/set angle */
void TestMapfileParser::testAngle() {
  MapfileParser * p  = new MapfileParser();
  QVERIFY(p->getAngle() == 0);
  delete p;
  p = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->getAngle() == 45);
  p->setAngle(90);
  QVERIFY(p->getAngle() == 90);
  delete p;

}

/** test template pattern */
void TestMapfileParser::testTemplatePattern() {
  MapfileParser * p  = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->getTemplatePattern() == "(/a-zA-Z0-9/)");
  p->setTemplatePattern("(/a-z/)");
  QVERIFY(p->getTemplatePattern() == "(/a-z/)");
  delete p;
}

/* test data pattern */
void TestMapfileParser::testDataPattern() {
  MapfileParser * p  = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->getDataPattern() == "(/a-zA-Z0-9/)");
  p->setDataPattern("(/a-z/)");
  QVERIFY(p->getDataPattern() == "(/a-z/)");
  delete p;
}

/** test mapfile path */
void TestMapfileParser::testMapfilePath() {
  MapfileParser * p  = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(QDir(p->getMapfilePath()).exists());
  delete p;

  // on a brand new (not saved yet) mapfile
  p = new MapfileParser();
  QVERIFY(p->getMapfilePath().isEmpty());
  delete p;

}

void TestMapfileParser::testImageColor() {
  MapfileParser * p = new MapfileParser();

  QList<int> l = p->getImageColor();
  QVERIFY(l.size() == 3);
  QVERIFY(l[0] == 0xff);
  QVERIFY(l[1] == 0xff);
  QVERIFY(l[2] == 0xff);

  // TODO: what if setting to integers so that the colour could not fit ? (e.g. > 0xff)
  p->setImageColor(0xddddde, 0xad, 0xef);
  l = p->getImageColor();
  QVERIFY(l.size() == 3);
  QVERIFY(l[0] == 0xddddde);
  QVERIFY(l[1] == 0xad);
  QVERIFY(l[2] == 0xef);



  delete p;

  p = new MapfileParser("../data/world_mapfile.map");

  l = p->getImageColor();
  QVERIFY(l.size() == 3);
  QVERIFY(l[0] == 0xfa);
  QVERIFY(l[1] == 0x00);
  QVERIFY(l[2] == 0x00);
  delete p;

}
