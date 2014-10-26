#ifndef SETMAPPROJECTIONCOMMAND_H
#define SETMAPPROJECTIONCOMMAND_H

#include <QUndoCommand>

#include "../parser/mapfileparser.h"

class SetMapProjectionCommand : public QUndoCommand {

 public:
   SetMapProjectionCommand(QString projection, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   QString oldProjection;
   QString newProjection;
   MapfileParser * parser;
};


#endif // SETMAPPROJECTIONCOMMAND_H
