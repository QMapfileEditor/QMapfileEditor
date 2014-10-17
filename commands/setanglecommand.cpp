#include "setanglecommand.h"

SetAngleCommand::SetAngleCommand(float newAngle, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newAngle(newAngle), parser(parser)
{
  oldAngle = parser->getAngle();

  setText(QObject::tr("change map angle to '%1'").arg(newAngle));
  parser->setAngle(newAngle);
}

void SetAngleCommand::undo(void) {
  parser->setAngle(oldAngle);
}

void SetAngleCommand::redo(void) {
  parser->setAngle(newAngle);
}

