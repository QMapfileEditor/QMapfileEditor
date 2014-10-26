#include "settemplatepatterncommand.h"

SetTemplatePatternCommand::SetTemplatePatternCommand(QString newPattern, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newPattern(newPattern), parser(parser)
{
   oldPattern = parser->getTemplatePattern();

   setText(QObject::tr("change template pattern to '%1'").arg(this->newPattern));
   parser->setTemplatePattern(this->newPattern);
}

void SetTemplatePatternCommand::undo(void) {
  parser->setTemplatePattern(this->oldPattern);
}

void SetTemplatePatternCommand::redo(void) {
  parser->setTemplatePattern(this->newPattern);
}

