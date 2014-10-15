#ifndef SETMAPMAXSIZECOMMAND_H
#define SETMAPMAXSIZECOMMAND_H

#include <QUndoCommand>

#include "../mapfileparser.h"

class SetMapMaxSizeCommand : public QUndoCommand {

 public:
   SetMapMaxSizeCommand(int newMaxSize, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   int newMaxSize, oldMaxSize;
   MapfileParser * parser;
};


#endif // SETMAPMAXSIZECOMMAND_H

