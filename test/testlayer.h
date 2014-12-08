#ifndef TESTLAYER_H
#define TESTLAYER_H

#include "autotest.h"


class TestLayer: public QObject
{
  Q_OBJECT
      private slots:
        void testLayer(void);

};

DECLARE_TEST(TestLayer)


#endif // TESTLAYER_H
