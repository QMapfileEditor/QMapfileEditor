#include "setshapepathcommand.h"

SetShapePathCommand::SetShapePathCommand(QString newShapePath, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newShapePath(newShapePath), parser(parser)
{
   oldShapePath = parser->getShapepath();

   setText(QObject::tr("change shapepath to '%1'").arg(this->newShapePath));
   parser->setShapepath(this->newShapePath);
}

void SetShapePathCommand::undo(void) {
  parser->setShapepath(this->oldShapePath);
}

void SetShapePathCommand::redo(void) {
  parser->setShapepath(this->newShapePath);
}

