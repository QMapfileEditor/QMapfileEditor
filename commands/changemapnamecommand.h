#ifndef CHANGEMAPNAMECOMMAND_H
#define CHANGEMAPNAMECOMMAND_H

#include <QUndoCommand>

#include "../parser/mapfileparser.h"

class ChangeMapNameCommand : public QUndoCommand {

 public:
   ChangeMapNameCommand(QString, MapfileParser *, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   QString oldName;
   QString newName;
   MapfileParser * parser;
};


#endif // CHANGEMAPNAMECOMMAND_H

