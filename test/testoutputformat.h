#ifndef TESTOUTPUTFORMAT_H
#define TESTOUTPUTFORMAT_H

#include "autotest.h"


class TestOutputFormat : public QObject {
  Q_OBJECT
      private slots:
      void testInitOutputFormat();
};


DECLARE_TEST(TestOutputFormat)


#endif // TESTOUTPUTFORMAT_H
