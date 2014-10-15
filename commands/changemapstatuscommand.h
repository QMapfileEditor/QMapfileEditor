#ifndef CHANGEMAPSTATUSCOMMAND_H
#define CHANGEMAPSTATUSCOMMAND_H

#include <QUndoCommand>

#include "../mapfileparser.h"

class ChangeMapStatusCommand : public QUndoCommand {

 public:
   ChangeMapStatusCommand(bool newStatus, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   bool oldStatus;
   bool newStatus;
   MapfileParser * parser;
};


#endif // CHANGEMAPSTATUSCOMMAND_H

