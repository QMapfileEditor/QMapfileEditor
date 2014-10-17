#include "setsymbolsetcommand.h"

SetSymbolSetCommand::SetSymbolSetCommand(QString newSymbolSet, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newSymbolSet(newSymbolSet), parser(parser)
{
   oldSymbolSet = parser->getSymbolSet();

   setText(QObject::tr("change symbolset to '%1'").arg(this->newSymbolSet));
   parser->setSymbolSet(this->newSymbolSet);
}

void SetSymbolSetCommand::undo(void) {
  parser->setSymbolSet(this->oldSymbolSet);
}

void SetSymbolSetCommand::redo(void) {
  parser->setSymbolSet(this->newSymbolSet);
}

