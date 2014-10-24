#ifndef SETRESOLUTIONCOMMAND_H
#define SETRESOLUTIONCOMMAND_H

#include <QUndoCommand>

#include "../parser/mapfileparser.h"

class SetResolutionCommand : public QUndoCommand {

 public:
   SetResolutionCommand(double newMapResolution, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   double newMapResolution, oldMapResolution;
   MapfileParser * parser;
};


#endif // SETRESOLUTIONCOMMAND_H

