#include "changemapstatuscommand.h"

ChangeMapStatusCommand::ChangeMapStatusCommand(bool newStatus, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newStatus(newStatus), parser(parser)
{
   oldStatus = parser->getMapStatus();
   setText(QObject::tr("change map status to '%1'").arg(this->newStatus));
   parser->setMapStatus(this->newStatus);
}

void ChangeMapStatusCommand::undo(void) {
  parser->setMapStatus(this->oldStatus);
}

void ChangeMapStatusCommand::redo(void) {
  parser->setMapStatus(this->newStatus);
}

