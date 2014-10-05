#ifndef TESTMAPFILEPARSER_H
#define TESTMAPFILEPARSER_H

#include "autotest.h"


class TestMapfileParser: public QObject
{
  Q_OBJECT
      private slots:
      void testInitMapfileParser();
      void testMapName();
      void testFilePath();
      void testGetCurrentMapImage();
      void testLayers();
      void testStatus();
      void testWidthHeight();
      void testMapMaxSize();
      void testMapUnits();
      void testImageType();
      void testMapExtent();
      void testDebug();
      void testConfigOptions();
      void testMetadata();
      void testShapePath();
      void testSymbolSet();
      void testFontSet();
      void testResolution();
      void testDefResolution();
      void testAngle();

};

DECLARE_TEST(TestMapfileParser)


#endif // TESTMAPFILEPARSER_H
