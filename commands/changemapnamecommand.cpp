#include "changemapnamecommand.h"

ChangeMapNameCommand::ChangeMapNameCommand(QString title, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), parser(parser), newName(title)
{
   oldName = parser->getMapName();

   setText(QObject::tr("change title to '%1'").arg(this->newName));
   parser->setMapName(this->newName);
}

void ChangeMapNameCommand::undo(void) {
  parser->setMapName(this->oldName);
}

void ChangeMapNameCommand::redo(void) {
  parser->setMapName(this->newName);
}

