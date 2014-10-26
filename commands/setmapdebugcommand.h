#ifndef SETMAPDEBUGCOMMAND_H
#define SETMAPDEBUGCOMMAND_H

#include <QUndoCommand>

#include "../parser/mapfileparser.h"

class SetMapDebugCommand : public QUndoCommand {

 public:
   SetMapDebugCommand(int newDebug, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   int newDebug, oldDebug;
   MapfileParser * parser;
};


#endif // SETMAPDEBUGCOMMAND_H

