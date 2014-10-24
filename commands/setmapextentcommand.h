#ifndef SETMAPEXTENTCOMMAND_H
#define SETMAPEXTENTCOMMAND_H

#include <QUndoCommand>

#include "../parser/mapfileparser.h"

class SetMapExtentCommand : public QUndoCommand {

 public:
   SetMapExtentCommand(double mx, double my, double Mx, double My, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   double newmx, newmy, newMx, newMy, oldmx, oldmy, oldMx, oldMy;
   MapfileParser * parser;
};


#endif // SETMAPEXTENTCOMMAND_H

