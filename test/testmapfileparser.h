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

};

DECLARE_TEST(TestMapfileParser)


#endif // TESTMAPFILEPARSER_H
