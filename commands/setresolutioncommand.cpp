#include "setresolutioncommand.h"

SetResolutionCommand::SetResolutionCommand(double newMapResolution, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newMapResolution(newMapResolution), parser(parser)
{
  oldMapResolution = parser->getResolution();

   setText(QObject::tr("change map resolution to '%1'").arg(newMapResolution));
   parser->setResolution(newMapResolution);
}

void SetResolutionCommand::undo(void) {
  parser->setResolution(oldMapResolution);
}

void SetResolutionCommand::redo(void) {
  parser->setResolution(newMapResolution);
}

