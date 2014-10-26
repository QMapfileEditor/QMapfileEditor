#ifndef SETTEMPLATEPATTERNCOMMAND_H
#define SETTEMPLATEPATTERNCOMMAND_H

#include <QUndoCommand>

#include "../parser/mapfileparser.h"

class SetTemplatePatternCommand : public QUndoCommand {

 public:
   SetTemplatePatternCommand(QString newPattern, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   QString oldPattern;
   QString newPattern;
   MapfileParser * parser;
};


#endif // SETTEMPLATEPATTERNCOMMAND_H
