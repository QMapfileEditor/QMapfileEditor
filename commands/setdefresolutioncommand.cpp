#include "setdefresolutioncommand.h"

SetDefResolutionCommand::SetDefResolutionCommand(double newDefResolution, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newDefResolution(newDefResolution), parser(parser)
{
  oldDefResolution = parser->getDefResolution();

   setText(QObject::tr("change def resolution to '%1'").arg(newDefResolution));
   parser->setDefResolution(newDefResolution);
}

void SetDefResolutionCommand::undo(void) {
  parser->setDefResolution(oldDefResolution);
}

void SetDefResolutionCommand::redo(void) {
  parser->setDefResolution(newDefResolution);
}

