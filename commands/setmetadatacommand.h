#ifndef SETMETADATACOMMAND_H
#define SETMETADATACOMMAND_H

#include <QUndoCommand>

#include "../parser/mapfileparser.h"

class SetMetadataCommand : public QUndoCommand {

 public:
   SetMetadataCommand(QString key, QString newValue, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   QString key;
   QString oldValue;
   QString newValue;
   MapfileParser * parser;
};


#endif // SETMETADATACOMMAND_H
