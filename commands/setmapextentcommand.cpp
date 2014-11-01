/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: 
 * Author: Pierre Mauduit
 *
 **********************************************************************
 * Copyright (c) 2014, Pierre Mauduit
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of this Software or works derived from this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 ****************************************************************************/

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

