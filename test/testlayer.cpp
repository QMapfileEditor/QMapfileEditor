#include "testlayer.h"

#include "../parser/layer.h"
#include "../parser/mapfileparser.h"

#include <QDebug>

/** simple test case to check the parsing
 * of layers in an existing mapfile.
 */

void TestLayer::testLayer()
{
  MapfileParser * p = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->isLoaded());


  QList<Layer *> layers = p->getLayers();

  QVERIFY(layers.size() == 2);

  Layer * firstlayer = layers.at(0);

  QVERIFY(firstlayer->getStatus() == 1); // MS_ON
  QVERIFY(firstlayer->getRequires() == "World contour"); // REQUIRES "World contour"
  QVERIFY(firstlayer->getGroup() == "common"); // GROUP common
  QVERIFY(firstlayer->getType() == "MS_LAYER_RASTER");
  QVERIFY(firstlayer->getOpacity() == 20);
  QVERIFY(firstlayer->getMask() == "");
  QVERIFY(firstlayer->getUnits() == "meters");
  QVERIFY(firstlayer->getSizeUnits() == "pixels");
  QVERIFY(firstlayer->getMinX() == -180);
  QVERIFY(firstlayer->getMaxX() ==  180);
  QVERIFY(firstlayer->getMinY() == -90);
  QVERIFY(firstlayer->getMaxY() ==  90);
  QVERIFY(firstlayer->getMinScale() == -1);
  QVERIFY(firstlayer->getMaxScale() == -1);
  QVERIFY(firstlayer->getPlugin() == "");
  QVERIFY(firstlayer->getTolerance() == -1);
  QVERIFY(firstlayer->getToleranceUnits() == "");
  QVERIFY(firstlayer->getMaxGeoWidth() == -1);
  QVERIFY(firstlayer->getClassGroup() == "");
  QVERIFY(firstlayer->getTemplate() == "test.html");
  QVERIFY(firstlayer->getHeader() == "header.html");
  QVERIFY(firstlayer->getFooter() == "footer.html");
  QVERIFY(firstlayer->getStyleItem() == "");
  QVERIFY(firstlayer->getFilterItem() == "");
  QVERIFY(firstlayer->getLabelItem() == "");
  QVERIFY(firstlayer->getClassItem() == "");
  QVERIFY(firstlayer->getSymbolScaleDenom() == -1);
  QVERIFY(firstlayer->getLabelCache() == true);
  QVERIFY(firstlayer->getPostLabelCache() == false);
  QVERIFY(firstlayer->getDebugLevel() == 0);
  QVERIFY(firstlayer->getLabelRequires() == "");
  QVERIFY(firstlayer->getMaxScaleDenomLabel() == -1);
  QVERIFY(firstlayer->getMinScaleDenomLabel() == -1);


  delete p;

}


