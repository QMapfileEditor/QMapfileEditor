#ifndef SETMAPSIZECOMMAND_H
#define SETMAPSIZECOMMAND_H

#include <QUndoCommand>

#include "../parser/mapfileparser.h"

class SetMapSizeCommand : public QUndoCommand {

 public:
   SetMapSizeCommand(int newWidth, int newHeight, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   int newWidth, newHeight, oldWidth, oldHeight;
   MapfileParser * parser;
};


#endif // SETMAPSIZECOMMAND_H

