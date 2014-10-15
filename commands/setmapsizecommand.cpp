#include "setmapsizecommand.h"

SetMapSizeCommand::SetMapSizeCommand(int newWidth, int newHeight, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newWidth(newWidth), newHeight(newHeight), parser(parser)
{
   oldWidth = parser->getMapWidth();
   oldHeight = parser->getMapHeight();

   setText(QObject::tr("change map size to '%1:%2'").arg(newWidth).arg(newHeight));
   parser->setMapSize(newWidth, newHeight);
}

void SetMapSizeCommand::undo(void) {
  parser->setMapSize(oldWidth, oldHeight);
}

void SetMapSizeCommand::redo(void) {
  parser->setMapSize(newWidth, newHeight);
}

