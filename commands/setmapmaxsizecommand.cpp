#include "setmapmaxsizecommand.h"

SetMapMaxSizeCommand::SetMapMaxSizeCommand(int newMaxSize, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newMaxSize(newMaxSize), parser(parser)
{
  oldMaxSize = parser->getMapMaxsize();

   setText(QObject::tr("change map max size to '%1'").arg(newMaxSize));
   parser->setMapMaxsize(newMaxSize);
}

void SetMapMaxSizeCommand::undo(void) {
  parser->setMapMaxsize(oldMaxSize);
}

void SetMapMaxSizeCommand::redo(void) {
  parser->setMapMaxsize(newMaxSize);
}

