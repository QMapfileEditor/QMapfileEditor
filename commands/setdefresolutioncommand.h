#ifndef SETDEFRESOLUTIONCOMMAND_H
#define SETDEFRESOLUTIONCOMMAND_H

#include <QUndoCommand>

#include "../parser/mapfileparser.h"

class SetDefResolutionCommand : public QUndoCommand {

 public:
   SetDefResolutionCommand(double newDefResolution, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   double newDefResolution, oldDefResolution;
   MapfileParser * parser;
};


#endif // SETDEFRESOLUTIONCOMMAND_H

