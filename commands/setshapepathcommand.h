#ifndef SETSHAPEPATHCOMMAND_H
#define SETSHAPEPATHCOMMAND_H

#include <QUndoCommand>

#include "../parser/mapfileparser.h"

class SetShapePathCommand : public QUndoCommand {

 public:
   SetShapePathCommand(QString newShapePath, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   QString oldShapePath;
   QString newShapePath;
   MapfileParser * parser;
};


#endif // SETSHAPEPATHCOMMAND_H

