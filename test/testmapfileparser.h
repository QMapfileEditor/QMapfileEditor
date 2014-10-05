#include <QtTest/QtTest>

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
