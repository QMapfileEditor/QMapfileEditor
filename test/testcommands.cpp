#include "testcommands.h"
#include "../commands/changemapnamecommand.h"

void TestCommands::testChangeMapNameCommand(void) {
  MapfileParser *p = new MapfileParser();

  // initially, on a blank mapfile, name == MS
  QVERIFY(p->getMapName() == "MS");

  ChangeMapNameCommand *c = new ChangeMapNameCommand("MS2", p);

  QVERIFY(p->getMapName() == "MS2");

  c->undo();

  
  QVERIFY(p->getMapName() == "MS");

  c->redo();


  QVERIFY(p->getMapName() == "MS2");

  if (p) delete p;
  if (c) delete c;

}
