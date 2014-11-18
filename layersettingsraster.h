/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: Settings of Raster Layer Object
 * Author: Pierre Mauduit / Yves Jacolin
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

#ifndef LAYERSETTINGSRASTER_H
#define LAYERSETTINGSRASTER_H

#include <QAbstractListModel>
#include <QColorDialog>
#include <QDataWidgetMapper>
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>

#include "keyvaluemodel.h"
#include "commands/changemapnamecommand.h"
#include "parser/mapfileparser.h"
#include "parser/layer.h"

namespace Ui {
class LayerSettingsRaster;
}

class LayerSettingsRaster : public QTabWidget
{
  Q_OBJECT

 public:
      explicit LayerSettingsRaster(QWidget * parent, MapfileParser *m, Layer * l);
      ~LayerSettingsRaster();

 public slots:
      void accept();

 private:
      Ui::LayerSettingsRaster * ui;
      MapfileParser * mapfile;
      Layer * layer;

};

#endif // LAYERSETTINGSRASTER_H

