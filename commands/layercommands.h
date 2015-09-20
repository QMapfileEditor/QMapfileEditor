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

#ifndef LAYERCOMMANDS_H
#define LAYERCOMMANDS_H

#include <QUndoCommand>

#include "../mainwindow.h"

class AddLayerCommand : public QUndoCommand {

 public:
   AddLayerCommand(QString &layerName, bool isRaster, MainWindow * wnd, QUndoCommand *parent = 0);
   ~AddLayerCommand();
   void undo();
   void redo();

 private:
   QString layerName;
   bool isRaster;
   MainWindow * mainwindow;
};

class RemoveLayerCommand : public QUndoCommand {

 public:
   RemoveLayerCommand(Layer * deletedLayer, MainWindow *wnd, QUndoCommand *parent = 0);
   ~RemoveLayerCommand();
   void undo();
   void redo();

 private:
   Layer * deletedLayer;
   MainWindow * mainwindow;
};

class ChangeLayerNameCommand : public QUndoCommand {

 public:
   ChangeLayerNameCommand(Layer *l, QString &oldLayerName, QString &newLayerName, QUndoCommand *parent = 0);
   ~ChangeLayerNameCommand();
   void undo();
   void redo();

 private:
   QString oldLayerName, newLayerName;
   Layer *modifiedLayer;
};

class ChangeLayerStatusCommand : public QUndoCommand {

 public:
   ChangeLayerStatusCommand(Layer *l, int oldStatus, int newStatus, QUndoCommand *parent = 0);
   ~ChangeLayerStatusCommand();
   void undo();
   void redo();

 private:
   int oldStatus, newStatus;
   Layer *modifiedLayer;
};

class ChangeLayerRequiresCommand : public QUndoCommand {

 public:
   ChangeLayerRequiresCommand(Layer *, QString &, QString &, QUndoCommand * parent = 0);
   ~ChangeLayerRequiresCommand();
   void undo();
   void redo();

 private:
   QString oldLayer, newLayer;
   Layer *modifiedLayer;
};


#endif // LAYERCOMMANDS_H
