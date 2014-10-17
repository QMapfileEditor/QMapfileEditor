#include "setfontsetcommand.h"

SetFontSetCommand::SetFontSetCommand(QString newFontSet, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newFontSet(newFontSet), parser(parser)
{
   oldFontSet = parser->getFontSet();

   setText(QObject::tr("change fontset to '%1'").arg(this->newFontSet));
   parser->setFontSet(this->newFontSet);
}

void SetFontSetCommand::undo(void) {
  parser->setFontSet(this->oldFontSet);
}

void SetFontSetCommand::redo(void) {
  parser->setFontSet(this->newFontSet);
}

