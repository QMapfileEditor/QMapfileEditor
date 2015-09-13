/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: Main windows
 * Author: Pierre Mauduit / Yves Jacolin
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <cmath>

#include <QDir>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPixmap>
#include <QResizeEvent>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QUndoStack>
#include <QUndoView>

#include "mapscene.h"
#include "mapsettings.h"
#include "fontsettings.h"
#include "layersettingsvector.h"
#include "layersettingsraster.h"
#include "commands/layercommands.h"
#include "parser/mapfileparser.h"
#include "parser/layer.h"


namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
      explicit MainWindow(QWidget *parent = 0);

      void openMapfile(const QString &);
      void pushUndoStack(QUndoCommand *);
      QString mapfilename, prevDirPath = NULL;
      QDir mapfiledir;

      QStringList gdalGdalDrivers;
      QStringList gdalOgrDrivers;

      // Layers-related actions
      void removeLayer(const QString & layerName);
      void addLayer(const QString & layerName, bool isRaster);
      void removeLayer(const Layer *);
      void addLayer(const Layer *);

      QUndoStack * getUndoStack() const;

      ~MainWindow();

 public slots:
      void addLayerVectorTriggered();
      void addLayerRasterTriggered();
      void handleUndoStackChanged(int);
      void openMapfile();
      void newMapfile();
      void panPreview(qreal,qreal);
      void panToggled(bool);
      void removeLayerTriggered();
      void saveMapfile();
      void saveAsMapfile();
      void showAbout();
      void showInfo(const QString & message);
      void showLayerSettings(const QModelIndex &);
      void showLayerSettings(void);
      void showMapSettings();
      void showFontSettings();
      void showUndoStack();
      void updateMapPreview(void);
      void zoomMapPreview(QRectF);
      void zoomOutMapPreview();
      void zoomToggled(bool);
      void zoomToOriginalExtent();
      void zoom2Toggled(bool);


 private:
      Ui::MainWindow *ui;

      MapfileParser * mapfile = NULL;

      // Dialog which handles the mapfile settings
      MapSettings * settings = NULL;
      FontSettings * fontSettings = NULL;
      QDialog * layerSettingsDialog = NULL;
      QDialogButtonBox *buttonBox;

      QUndoStack * undoStack;
      QUndoView  * undoView = NULL;

      void addLayerTriggered(bool);
      // internal methods
      void reinitMapfile();
      void updateMapPreview(const int &, const int &);
      QMessageBox::StandardButton warnIfActiveSession(void);

      double currentMapMinX, currentMapMaxX, currentMapMinY, currentMapMaxY;

      LayerModel * layerModel;
};

#endif // MAINWINDOW_H
