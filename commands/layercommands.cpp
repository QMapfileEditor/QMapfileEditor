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

#include "layercommands.h"

// Add layer command

AddLayerCommand::AddLayerCommand(Layer *newLayer, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), parser(parser)
{
  this->newLayer = new Layer(* newLayer);
  setText(QObject::tr("Create new layer '%1'").arg(newLayer->getName()));
}

void AddLayerCommand::undo(void) {
  parser->removeLayer(newLayer);
}

void AddLayerCommand::redo(void) {
  parser->addLayer(newLayer);
}

AddLayerCommand::~AddLayerCommand() {
  delete newLayer;
}

// Remove layer command

RemoveLayerCommand::RemoveLayerCommand(Layer *deletedLayer, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), parser(parser)
{
  this->deletedLayer = new Layer(* deletedLayer);
  setText(QObject::tr("Delete layer '%1'").arg(deletedLayer->getName()));
}

void RemoveLayerCommand::undo(void) {
  parser->addLayer(deletedLayer);
}

void RemoveLayerCommand::redo(void) {
  parser->removeLayer(deletedLayer);
}

RemoveLayerCommand::~RemoveLayerCommand() {
  delete deletedLayer;
}


// Edit layers commands


