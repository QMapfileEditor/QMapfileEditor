#include "setmapextentcommand.h"

SetMapExtentCommand::SetMapExtentCommand(double x1, double y1, double x2, double y2, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newmx(x1), newmy(y1), newMx(x2), newMy(y2), parser(parser)
{

   oldmx = parser->getMapExtentMinX();
   oldmy = parser->getMapExtentMinY();
   oldMx = parser->getMapExtentMaxX();
   oldMy = parser->getMapExtentMaxY();


   setText(QObject::tr("change map extent to '%1:%2:%3:%4'").arg(x1).arg(y1).arg(x2).arg(y2));
   parser->setMapExtent(x1,y1,x2,y2);
}

void SetMapExtentCommand::undo(void) {
   parser->setMapExtent(oldmx,oldmy,oldMx,oldMy);
}

void SetMapExtentCommand::redo(void) {
   parser->setMapExtent(newmx,newmy,newMx,newMy);
}

