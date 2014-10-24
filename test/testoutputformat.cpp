#include "testoutputformat.h"
#include "../parser/outputformat.h"

/** test OutputFormat constructor */

void TestOutputFormat::testInitOutputFormat() {
   OutputFormat * f = new OutputFormat("myFormat", "image/png", "AGG/PNG", "png", 0, true);

   QVERIFY(f);

   QVERIFY(f->getName() == "myFormat");
   QVERIFY(f->getMimeType() == "image/png");
   QVERIFY(f->getDriver() == "AGG/PNG");
   QVERIFY(f->getImageMode() == 0);
   QVERIFY(f->getTransparent());

   f->setName("myNewFormat");
   f->setMimeType("image/jpeg");
   f->setDriver("gd/jpg");
   f->setImageMode(1);
   f->setTransparent(false);

   QVERIFY(f->getName() == "myNewFormat");
   QVERIFY(f->getMimeType() == "image/jpeg");
   QVERIFY(f->getDriver() == "gd/jpg");
   QVERIFY(f->getImageMode() == 1);
   QVERIFY(f->getTransparent() == false);

   if (f) delete f;

   // Test creating on the stack
   OutputFormat fs = OutputFormat();
   QVERIFY(fs.isEmpty());

}


