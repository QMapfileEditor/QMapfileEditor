#include <QtTest/QtTest>

class TestMapfileParser: public QObject
{
  Q_OBJECT
      private slots:
      void initMapfileParser();
      void testMapName();
      void testFilePath();
};
