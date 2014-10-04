#ifndef CHANGEMAPNAMECOMMAND_H
#define CHANGEMAPNAMECOMMAND_H

#include <QUndoCommand>

#include "../mapfileparser.h"

class ChangeMapNameCommand : public QUndoCommand {

 public:
   ChangeMapNameCommand(QString newName, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   QString oldName;
   QString newName;
   MapfileParser * parser;
};


#endif // CHANGEMAPNAMECOMMAN_H

