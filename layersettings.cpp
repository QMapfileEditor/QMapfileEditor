/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * 
 * Purpose: Settings of Layer window, code common to vector|raster layers
 *
 * Author: Pierre Mauduit, Yves Jacolin
 *
 **********************************************************************
 * Copyright (c) 2014, Yves Jacolin
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
#include "layersettings.h"
#include "mainwindow.h"


LayerSettings::LayerSettings(QWidget *parent, MapfileParser *p, Layer *l):
   QTabWidget(parent), mapfile(p), layer(l) {};

void LayerSettings::initStatusRadioButton(QRadioButton * on, QRadioButton * off, QRadioButton * defaultRadio) {
  if(layer->getStatus() == 1) {
    on->setChecked(true);
    off->setChecked(false);
    defaultRadio->setChecked(false);
  } else if (layer->getStatus() == 2) {
    on->setChecked(false);
    off->setChecked(false);
    defaultRadio->setChecked(true);
  } else {
    on->setChecked(false);
    off->setChecked(true);
    defaultRadio->setChecked(false);
  }
}

void LayerSettings::initRequiresMaskCombo(QComboBox *requires, QComboBox *mask) {
  QStringList appropriateLayerList = mapfile->getLayerList();
  appropriateLayerList.prepend(QString());
  appropriateLayerList.removeAll(layer->getName());

  requires->addItems(appropriateLayerList);
  requires->setCurrentIndex(requires->findText(layer->getRequires()));

  mask->addItems(appropriateLayerList);
  mask->setCurrentIndex(mask->findText(layer->getMask()));
}

/**
 * saves layer settings, code shared by vector and raster windows
 */
void LayerSettings::accept() {
  // Getting the QUndoStack from MainWindow
  QDialog * ls = (QDialog *) parent();
  MainWindow * mw = (MainWindow *) ls->parent();
  QUndoStack * stack = mw->getUndoStack();

  // Title changed

}

void LayerSettings::reject() {
  qDebug() << "reject()  in LayerSettings triggered";
}
