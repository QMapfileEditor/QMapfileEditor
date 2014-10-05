#ifndef TESTCOMMANDS_H
#define TESTCOMMANDS_H

// This class provides a testsuite for commands (Qt undo/redo framework)
// the tested classes are located onto the ../commands/ subdirectory.

#include "autotest.h"

class TestCommands : public QObject {
  Q_OBJECT
      private slots:
      void testChangeMapNameCommand();
};

DECLARE_TEST(TestCommands)


#endif // TESTCOMMANDS_H
