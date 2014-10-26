#include "setmetadatacommand.h"

SetMetadataCommand::SetMetadataCommand(QString key, QString value, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), key(key), newValue(value), parser(parser)
{
   oldValue = parser->getMetadata(key);

   setText(QObject::tr("change metadata[%1] to '%2'").arg(this->key).arg(this->newValue));
   parser->setMetadata(this->key, this->newValue);
}

void SetMetadataCommand::undo(void) {
  parser->setMetadata(this->key, this->oldValue);
}

void SetMetadataCommand::redo(void) {
  parser->setMetadata(this->key, this->newValue);
}

