#include "setmapprojectioncommand.h"

SetMapProjectionCommand::SetMapProjectionCommand(QString projection, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newProjection(projection), parser(parser)
{
   oldProjection = parser->getMapProjection();

   setText(QObject::tr("change projection to '%1'").arg(this->newProjection));
   parser->setMapProjection(this->newProjection);
}

void SetMapProjectionCommand::undo(void) {
  parser->setMapProjection(this->oldProjection);
}

void SetMapProjectionCommand::redo(void) {
  parser->setMapProjection(this->newProjection);
}

