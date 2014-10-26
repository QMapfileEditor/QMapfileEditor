#ifndef SETFONTSETCOMMAND_H
#define SETFONTSETCOMMAND_H

#include <QUndoCommand>

#include "../parser/mapfileparser.h"

class SetFontSetCommand : public QUndoCommand {

 public:
   SetFontSetCommand(QString newFontSet, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   QString oldFontSet;
   QString newFontSet;
   MapfileParser * parser;
};


#endif // SETFONTSETCOMMAND_H

