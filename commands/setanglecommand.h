#ifndef SETANGLECOMMAND_H
#define SETANGLECOMMAND_H

#include <QUndoCommand>

#include "../mapfileparser.h"

class SetAngleCommand : public QUndoCommand {

 public:
   SetAngleCommand(float newAngle, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   float newAngle, oldAngle;
   MapfileParser * parser;
};


#endif // SETANGLECOMMAND_H

