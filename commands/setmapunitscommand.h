#ifndef SETMAPUNITSCOMMAND_H
#define SETMAPUNITSCOMMAND_H

#include <QUndoCommand>

#include "../mapfileparser.h"

class SetMapUnitsCommand : public QUndoCommand {

 public:
   SetMapUnitsCommand(int newUnits, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   int newUnits, oldUnits;
   MapfileParser * parser;
};


#endif // SETMAPSIZECOMMAND_H

