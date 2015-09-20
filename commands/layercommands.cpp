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

#include <mapserver.h>

#include "layercommands.h"

// "Add layer" command

AddLayerCommand::AddLayerCommand(QString & layerName, bool isRaster, MainWindow *wnd, QUndoCommand *parent)
     : QUndoCommand(parent), layerName(layerName), isRaster(isRaster), mainwindow(wnd)
{
  setText(QObject::tr("Create new layer '%1'").arg(layerName));
}

void AddLayerCommand::undo(void) {
  mainwindow->removeLayer(layerName);
}

void AddLayerCommand::redo(void) {
  mainwindow->addLayer(layerName, isRaster);
}

AddLayerCommand::~AddLayerCommand() {}

// "Remove layer" command

RemoveLayerCommand::RemoveLayerCommand(Layer *deletedLayer, MainWindow *wnd, QUndoCommand *parent)
     : QUndoCommand(parent), mainwindow(wnd)
{
  this->deletedLayer = new Layer(* deletedLayer);
  setText(QObject::tr("Delete layer '%1'").arg(deletedLayer->getName()));
}

void RemoveLayerCommand::undo(void) {
  mainwindow->addLayer(deletedLayer);
}

void RemoveLayerCommand::redo(void) {
  mainwindow->removeLayer(deletedLayer);
}

RemoveLayerCommand::~RemoveLayerCommand() {
  delete deletedLayer;
}

// "Change layer name" command
ChangeLayerNameCommand::ChangeLayerNameCommand(Layer * modifiedLayer, QString & oldLayerName, QString & newLayerName, QUndoCommand * parent)
  : QUndoCommand(parent), oldLayerName(oldLayerName), newLayerName(newLayerName), modifiedLayer(modifiedLayer)
{
  setText(QObject::tr("Rename layer '%1' to '%2'").arg(newLayerName, oldLayerName));
}

void ChangeLayerNameCommand::undo(void) {
  modifiedLayer->setName(oldLayerName);
}

void ChangeLayerNameCommand::redo(void) {
  modifiedLayer->setName(newLayerName);
}

ChangeLayerNameCommand::~ChangeLayerNameCommand() {}

// "Change layer status" command
ChangeLayerStatusCommand::ChangeLayerStatusCommand(Layer * modifiedLayer, int oldStatus, int newStatus, QUndoCommand * parent)
  : QUndoCommand(parent), oldStatus(oldStatus), newStatus(newStatus), modifiedLayer(modifiedLayer)
{
  QString oldStatusDesc, newStatusDesc;
  if (oldStatus == MS_ON)
    oldStatusDesc = QObject::tr("ON");
  else if (oldStatus == MS_OFF)
    oldStatusDesc = QObject::tr("OFF");
  else
    oldStatusDesc = QObject::tr("Default");
  if (newStatus == MS_ON)
    newStatusDesc = QObject::tr("ON");
  else if (newStatus == MS_OFF)
    newStatusDesc = QObject::tr("OFF");
  else
    newStatusDesc = QObject::tr("Default");
  setText(QObject::tr("Change layer status from '%1' to '%2'").arg(oldStatusDesc, newStatusDesc));
}

void ChangeLayerStatusCommand::undo(void) {
  modifiedLayer->setStatus(oldStatus);
}

void ChangeLayerStatusCommand::redo(void) {
  modifiedLayer->setStatus(newStatus);
}

ChangeLayerStatusCommand::~ChangeLayerStatusCommand() {}

// "Change requires" command
ChangeLayerRequiresCommand::ChangeLayerRequiresCommand(Layer * modifiedLayer, QString oldLayer, QString newLayer, QUndoCommand *parent)
  : QUndoCommand(parent), oldLayer(oldLayer), newLayer(newLayer), modifiedLayer(modifiedLayer)  {
  setText(QObject::tr("Change layer require from '%1' to '%2'").arg(oldLayer, newLayer));
}

void ChangeLayerRequiresCommand::undo() {
  modifiedLayer->setRequires(oldLayer);
}

void ChangeLayerRequiresCommand::redo() {
  modifiedLayer->setRequires(newLayer);
}

ChangeLayerRequiresCommand::~ChangeLayerRequiresCommand() {}

