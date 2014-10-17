#ifndef SETSYMBOLSETCOMMAND_H
#define SETSYMBOLSETCOMMAND_H

#include <QUndoCommand>

#include "../mapfileparser.h"

class SetSymbolSetCommand : public QUndoCommand {

 public:
   SetSymbolSetCommand(QString newSymbolSet, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   QString oldSymbolSet;
   QString newSymbolSet;
   MapfileParser * parser;
};


#endif // SETSYMBOLSETCOMMAND_H

