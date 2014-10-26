#include "setmapdebugcommand.h"

SetMapDebugCommand::SetMapDebugCommand(int newDebug, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newDebug(newDebug), parser(parser)
{
  oldDebug = parser->getDebug();

   setText(QObject::tr("change map debug level to '%1'").arg(newDebug));
   parser->setDebug(newDebug);
}

void SetMapDebugCommand::undo(void) {
  parser->setDebug(oldDebug);
}

void SetMapDebugCommand::redo(void) {
  parser->setDebug(newDebug);
}

