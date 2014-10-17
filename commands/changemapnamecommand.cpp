#include "changemapnamecommand.h"

ChangeMapNameCommand::ChangeMapNameCommand(QString title, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newName(title), parser(parser)
{
   oldName = parser->getMapName();

   setText(QObject::tr("change title to '%1'").arg(newName));
   parser->setMapName(newName);
}

void ChangeMapNameCommand::undo(void) {
  parser->setMapName(this->oldName);
}

void ChangeMapNameCommand::redo(void) {
  parser->setMapName(this->newName);
}

