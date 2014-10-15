#include "setmapunitscommand.h"

SetMapUnitsCommand::SetMapUnitsCommand(int newUnits, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newUnits(newUnits), parser(parser)
{
   oldUnits = parser->getMapUnits();


   setText(QObject::tr("change map units to '%1'").arg(newUnits));
   parser->setMapUnits(newUnits);
}

void SetMapUnitsCommand::undo(void) {
  parser->setMapUnits(oldUnits);
}

void SetMapUnitsCommand::redo(void) {
  parser->setMapUnits(newUnits);
}

