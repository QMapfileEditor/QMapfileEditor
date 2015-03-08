/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: Settings of Map Object
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  this->setWindowIcon(QIcon(":/ui/images/icons/icon.png"));

  this->undoStack = new QUndoStack(this);

  this->showInfo(tr("Initializing default mapfile"));


  // inits QAction behaviours
  this->connect(ui->actionZoom_to_Extent,   SIGNAL(triggered()), SLOT(zoomToOriginalExtent()));
  this->connect(ui->actionZoom,   SIGNAL(toggled(bool)), SLOT(zoomToggled(bool)));
  this->connect(ui->actionZoom_2, SIGNAL(toggled(bool)), SLOT(zoom2Toggled(bool)));
  this->connect(ui->actionPan,    SIGNAL(toggled(bool)), SLOT(panToggled(bool)));

  this->connect(ui->actionShowUndoStack, SIGNAL(triggered()), this, SLOT(showUndoStack()));
  // inits the model for the mapfile structure
  //QStringListModel * mfStructureModel = new QStringListModel(this);
  ui->mf_structure->setEditTriggers(QAbstractItemView::NoEditTriggers);
  this->connect(ui->mf_structure, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(showLayerSettings(const QModelIndex &)));
  this->connect(ui->mf_editLayer, SIGNAL(clicked()), this, SLOT(showLayerSettings()));
  this->connect(ui->mf_addlayer, SIGNAL(clicked()), ui->mf_addlayer, SLOT(showMenu()));

  // inits the graphics scene
  MapScene * mapScene = new MapScene(this);
  ui->mf_preview->setScene(mapScene);
  ui->mf_preview->setSceneRect(0,0,this->ui->mf_preview->viewport()->width(),this->ui->mf_preview->viewport()->height());
  ui->mf_preview->scene()->clear();
  this->connect(mapScene, SIGNAL(notifyAreaToZoomIn(QRectF)), this, SLOT(zoomMapPreview(QRectF)));
  this->connect(mapScene, SIGNAL(notifyAreaToZoomOut()), this, SLOT(zoomOutMapPreview()));
  this->connect(mapScene, SIGNAL(notifyAreaToPan(qreal, qreal)), this, SLOT(panPreview(qreal, qreal)));


  // connects extra actions
  this->showInfo("Activate actions");
  this->connect(ui->actionNew,        SIGNAL(triggered()), SLOT(newMapfile()));
  this->connect(ui->actionOpen,       SIGNAL(triggered()), SLOT(openMapfile()));
  this->connect(ui->actionSave,       SIGNAL(triggered()), SLOT(saveMapfile()));
  this->connect(ui->actionSaveAs,     SIGNAL(triggered()), SLOT(saveAsMapfile()));
  this->connect(ui->actionMapSetting, SIGNAL(triggered()), SLOT(showMapSettings()));
  this->connect(ui->actionFont,        SIGNAL(triggered()), SLOT(showFontSettings()));
  this->connect(ui->actionAbout,      SIGNAL(triggered()), SLOT(showAbout()));
  this->connect(ui->actionRefresh,    SIGNAL(triggered()), SLOT(updateMapPreview()));

  // edit menu
  this->connect(ui->actionUndo, SIGNAL(triggered()), undoStack, SLOT(undo()));
  this->connect(ui->actionRedo, SIGNAL(triggered()), undoStack, SLOT(redo()));
  this->connect(undoStack, SIGNAL(indexChanged(int)), this, SLOT(handleUndoStackChanged(int)));

  //creates a default empty mapfileparser
  this->mapfile = new MapfileParser(QString());
  this->showInfo(tr("Initialisation process: success !"));

  // creates a Layer model
  this->layerModel = new LayerModel(this, this->mapfile->getLayers());
  ui->mf_structure->setModel(layerModel);
  for (int i = 1; i < layerModel->columnCount(); i++) {
      ui->mf_structure->hideColumn(i);
  }

  // menu for adding a new layer (vector/raster)
  QMenu * newLayerMenu = new QMenu(ui->mf_addlayer);
  newLayerMenu->addAction(ui->actionNew_vector_layer);
  newLayerMenu->addAction(ui->actionNew_raster_layer);
  ui->mf_addlayer->setMenu(newLayerMenu);

  // Add vector / raster, remove layer actions
  this->connect(ui->actionNew_vector_layer, SIGNAL(triggered()), SLOT(addLayerVectorTriggered()));
  this->connect(ui->actionNew_raster_layer, SIGNAL(triggered()), SLOT(addLayerRasterTriggered()));
  this->connect(ui->mf_removelayer, SIGNAL(clicked()), SLOT(removeLayerTriggered()));

}

// Undo / Redo related methods

void MainWindow::handleUndoStackChanged(int i) {
  Q_UNUSED(i);
  ui->actionUndo->setText(tr("Undo '%1'").arg(undoStack->undoText()));
  ui->actionRedo->setText(tr("Redo '%1'").arg(undoStack->redoText()));
}

// TODO separation of concerns: maybe just a getter
// for undostack, so that the other objects can
// push by themselves. (or else, a singleton)
void MainWindow::pushUndoStack(QUndoCommand *c) {
  this->undoStack->push(c);
  ui->actionSave->setEnabled(true);
}

void MainWindow::showUndoStack() {
  // open up undo stack window
  if (undoView == 0)
  {
    undoView = new QUndoView(undoStack);
    undoView->setWindowTitle(tr("Undo stack"));
    undoView->setAttribute(Qt::WA_QuitOnClose,false);
  }
  undoView->show();
}

// Zoom / Pan / ... map related methods

void MainWindow::zoomOutMapPreview() {
  double curmaxx = this->currentMapMaxX,
         curminx = this->currentMapMinX,
         curmaxy = this->currentMapMaxY,
         curminy = this->currentMapMinY;

  this->currentMapMinX -= std::abs(curmaxx - curminx) / 2;
  this->currentMapMinY -= std::abs(curmaxy - curminy) / 2;
  this->currentMapMaxX += std::abs(curmaxx - curminx) / 2;
  this->currentMapMaxY += std::abs(curmaxy - curminy) / 2;

  this->updateMapPreview();
}

void MainWindow::panPreview(qreal relatx, qreal relaty) {
  double maxx = this->currentMapMaxX, minx = this->currentMapMinX,
         maxy = this->currentMapMaxY, miny = this->currentMapMinY;

  float zonex = ui->mf_preview->sceneRect().width(),
        zoney = ui->mf_preview->sceneRect().height();

  double offsetx = ((maxx - minx) / zonex) * relatx;
  double offsety = ((maxy - miny) / zoney) * - relaty;

  this->currentMapMinX -= offsetx;
  this->currentMapMinY -= offsety;
  this->currentMapMaxX -= offsetx;
  this->currentMapMaxY -= offsety;

  this->updateMapPreview();

}

void MainWindow::zoomMapPreview(QRectF area) {
  // calculate final extent
  // Note: This algorithm is far from perfect but
  // should suffice as for now ...
  double maxy = this->currentMapMaxY;
  double miny = this->currentMapMinY;
  double maxx = this->currentMapMaxX;
  double minx = this->currentMapMinX;

  float rectminx = area.x(),
        rectmaxx = area.x() + area.width(),
        rectminy = area.y(),
        rectmaxy = area.y() + area.height();

  float zonex = ui->mf_preview->sceneRect().width(),
        zoney = ui->mf_preview->sceneRect().height();

  // First, establish the ratio (pixels / MS units)
  double x1beforetrans = (rectminx * (std::abs(maxx) + std::abs(minx))) / zonex;
  double x2beforetrans = (rectmaxx * (std::abs(maxx) + std::abs(minx))) / zonex;
  double y1beforetrans = (rectminy * (std::abs(maxy) + std::abs(miny))) / zoney;
  double y2beforetrans = (rectmaxy * (std::abs(maxy) + std::abs(miny))) / zoney;

  // Then translates so that coordinates are relative to MS origin
  // (and not Qt scene one)
  double x1trans = std::abs(minx) - x1beforetrans;
  double x2trans = std::abs(minx) - x2beforetrans;
  double y1trans = std::abs(miny) - y1beforetrans;
  double y2trans = std::abs(miny) - y2beforetrans;

  // saves the current extent of the map
  this->currentMapMinX = x1trans < x2trans ? x1trans: x2trans,
  this->currentMapMinY = y1trans < y2trans ? y1trans: y2trans,
  this->currentMapMaxX = x1trans >= x2trans ? x1trans: x2trans,
  this->currentMapMaxY = y1trans >= y2trans ? y1trans: y2trans;

  this->updateMapPreview();

}

void MainWindow::zoomToOriginalExtent() {
  // reinits the extent to its original value
  this->currentMapMinX = this->mapfile->getMapExtentMinX();
  this->currentMapMinY = this->mapfile->getMapExtentMinY();
  this->currentMapMaxX = this->mapfile->getMapExtentMaxX();
  this->currentMapMaxY = this->mapfile->getMapExtentMaxY();

  this->updateMapPreview();
}

void MainWindow::zoomToggled(bool toggle) {
  // notify MapScene that a zoom in action is possible or not
  ((MapScene *) this->ui->mf_preview->scene())->setZoomingIn(toggle);
  if (toggle == true) {
    this->ui->actionZoom_2->setChecked(false);
    this->ui->actionPan->setChecked(false);
  }
}

void MainWindow::zoom2Toggled(bool toggle) {
  ((MapScene *) this->ui->mf_preview->scene())->setZoomingOut(toggle);
  if (toggle == true) {
    this->ui->actionZoom->setChecked(false);
    this->ui->actionPan->setChecked(false);
  }
}

void MainWindow::panToggled(bool toggle) {
  ((MapScene *) this->ui->mf_preview->scene())->setPanning(toggle);
  if (toggle == true) {
    this->ui->actionZoom->setChecked(false);
    this->ui->actionZoom_2->setChecked(false);
  }
}

void MainWindow::reinitMapfile() {
  // if a MapSettings window has been opened, closes and destroys it
  if (this->settings) {
    this->settings->close();
    delete this->settings;
    this->settings = NULL;
  }
  // same for LayerSettings
  if (this->layerSettingsDialog) {
    this->layerSettingsDialog->close();
    delete this->layerSettingsDialog;
    this->layerSettingsDialog = NULL;
  }

  ((QStringListModel *) ui->mf_structure->model())->setStringList(QStringList());

  ui->mf_preview->scene()->clear();

  // Creates a new mapfileparser from scratch
  delete this->mapfile;
  this->mapfile = new MapfileParser(QString());

  // (re) init default extent
  this->currentMapMinX = this->mapfile->getMapExtentMinX();
  this->currentMapMinY = this->mapfile->getMapExtentMinY();
  this->currentMapMaxX = this->mapfile->getMapExtentMaxX();
  this->currentMapMaxY = this->mapfile->getMapExtentMaxY();
}


QMessageBox::StandardButton MainWindow::warnIfActiveSession() {
  return QMessageBox::question(this, tr("Warning: currently editing"),
                               tr("You are currently editing a mapfile. "
                                  "Opening another one will discard your "
                                  "current changes. Are you sure ?"),
                               QMessageBox::Yes | QMessageBox::No);
}

void MainWindow::newMapfile()
{
  // check if a mapfile is already opened
  // and modifications have been done
  if ((this->mapfile->isLoaded()) &&  (this->undoStack->count() > 0)) {
    if (warnIfActiveSession() == QMessageBox::Yes) {
      this->reinitMapfile();
    }
    else {
      return;
    }
  }
}

void MainWindow::openMapfile()
{
  QString prevFilePath = QDir::currentPath();
  if(this->prevDirPath != NULL) {
    prevFilePath = this->prevDirPath;
  }

  // TODO: check if current session has unsaved modifications instead
  if ((this->mapfile->isLoaded()) &&  (this->undoStack->count() > 0)) {
    if (warnIfActiveSession() == QMessageBox::No)
      return;
  }

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open map File"), prevFilePath, tr("Map file (*.map);; XML Map file (*.xml)"));

  QFileInfo mapFileInfo(fileName);
  this->mapfilename = mapFileInfo.fileName();
  this->mapfiledir = mapFileInfo.absolutePath();
  this->prevDirPath = mapFileInfo.absolutePath();

  this->openMapfile(fileName);
}

void MainWindow::openMapfile(const QString & mapfilePath) {
  if (mapfilePath.isEmpty()) {
    return;
  }

  // Reinit / free objects if necessary
  if (this->mapfile) {
    this->reinitMapfile();
  }

  this->mapfile = new MapfileParser(mapfilePath);
  this->layerModel->setLayers(this->mapfile->getLayers());

  if (! this->mapfile->isLoaded()) {
    QMessageBox::critical(
        this,
        "QMapfileEditor",
        tr("Error occured while loading the mapfile.")
        );
    this->reinitMapfile();
    this->showInfo("");
    this->layerModel->setLayers(this->mapfile->getLayers());
    return;
  }

  ui->mf_structure->setModel(layerModel);
  // inits the default extent
  this->currentMapMinX = this->mapfile->getMapExtentMinX();
  this->currentMapMinY = this->mapfile->getMapExtentMinY();
  this->currentMapMaxX = this->mapfile->getMapExtentMaxX();
  this->currentMapMaxY = this->mapfile->getMapExtentMaxY();

  this->updateMapPreview();

}


void MainWindow::updateMapPreview(void) {
  this->ui->mf_preview->setSceneRect(0,0,this->ui->mf_preview->viewport()->width(),
                                     this->ui->mf_preview->viewport()->height());
  int w = this->ui->mf_preview->frameSize().width(),
      h = this->ui->mf_preview->frameSize().height();
  this->updateMapPreview(w, h);
}

void MainWindow::updateMapPreview(const int & w, const int &h) {
  // re-init map preview
  this->ui->mf_preview->scene()->clear();
  // rendering the map
  QPixmap mapRepr = QPixmap();

  // Temporarily hack the map extent
  double tmpMinx, tmpMiny, tmpMaxx, tmpMaxy;
  tmpMinx = this->mapfile->getMapExtentMinX();
  tmpMiny = this->mapfile->getMapExtentMinY();
  tmpMaxx = this->mapfile->getMapExtentMaxX();
  tmpMaxy = this->mapfile->getMapExtentMaxY();

  this->mapfile->setMapExtent(this->currentMapMinX, this->currentMapMinY, this->currentMapMaxX, this->currentMapMaxY);

  // it is mapfileparser's class role to manage the allocated
  // memory
  unsigned char * mapImage = this->mapfile->getCurrentMapImage(w, h);
  int mapImageSize = this->mapfile->getCurrentMapImageSize();

  // Then restores the original map extent
 this->mapfile->setMapExtent(tmpMinx, tmpMiny, tmpMaxx, tmpMaxy);

  mapRepr.loadFromData(mapImage, mapImageSize);
  this->ui->mf_preview->scene()->addPixmap(mapRepr);
}

/**
 * Displays the map settings window.
 */
void MainWindow::showMapSettings() {
  // Mapfile not loaded
  if ((! this->mapfile) || (! this->mapfile->isLoaded())) {
    return;
  }
  // a window has already been created
  if (this->settings) {
    this->settings->reject();
    delete this->settings;
  }
  this->settings = new MapSettings(this, this->mapfile);
  this->settings->show();
}

/**
 * Displays the Font settings window.
 */
void MainWindow::showFontSettings() {
  // Mapfile not loaded
  if ((! this->mapfile) || (! this->mapfile->isLoaded())) {
    return;
  }
  // a window has already been created
  if (this->fontSettings) {
    this->fontSettings->reject();
    delete this->fontSettings;
  }
  this->fontSettings = new FontSettings(this, this->mapfile);
  this->fontSettings->show();
}

/**
 * Displays the layer settings window.
 */
void MainWindow::showLayerSettings(void) {
  this->showLayerSettings(this->ui->mf_structure->currentIndex());
}

void MainWindow::addLayerVectorTriggered() {
  addLayerTriggered(false);
}

void MainWindow::addLayerRasterTriggered() {
  addLayerTriggered(true);
}

void MainWindow::addLayerTriggered(bool isRaster) {
  // Generates a Layer name which is not already taken
  int i = 0;
  while (mapfile->layerExists(QString("NewLayer%1").arg(i))) { ++i; };

  QString newLayerName = QString("NewLayer%1").arg(i);

  // the pivot used between our Qt model and Mapserver is the name (char *)
  // to create a new layer, we need to ensure the name is not taken yet.

  this->undoStack->push(new AddLayerCommand(newLayerName, isRaster, this->mapfile));

  // refreshes the layerModel
  QList<Layer *> ls = this->mapfile->getLayers();
  this->layerModel->setLayers(ls);
}

void MainWindow::removeLayerTriggered() {
  // check validity of selection
  if ((! this->mapfile) || (! this->mapfile->isLoaded())) {
    return;
  }
  QModelIndex idx = this->ui->mf_structure->currentIndex();
  if ((idx.row() > this->mapfile->getLayers().length()) || (idx.row() < 0)) {
    return;
  }

  Layer * toRemove = this->mapfile->getLayers().at(idx.row());
  if (! toRemove) {
    return;
  }

  this->mapfile->removeLayer(toRemove);

  //refresh
  QList<Layer *> ls = this->mapfile->getLayers();
  this->layerModel->setLayers(ls);
}

void MainWindow::showLayerSettings(const QModelIndex &i) {
  if ((! this->mapfile) || (! this->mapfile->isLoaded())) {
    return;
  }

  if (this->layerSettingsDialog) {
    delete this->layerSettingsDialog;
    this->layerSettingsDialog = NULL;
  }

  this->layerSettingsDialog = new QDialog();
  this->layerSettingsDialog->setModal(true);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                      | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this->layerSettingsDialog, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this->layerSettingsDialog, SLOT(reject()));

  //TODO: give layer to layerSettings?
  //TODO: if layer is vector (ie TYPE = POINT, LINE or POLYGON and have no grid object) then:
  //this->showInfo(QString::number(i.model()->data(Qt::DisplayRole)));
  //qDebug() << i.row();
  if ((i.row() > this->mapfile->getLayers().length()) || (i.row() < 0))
    return;

  Layer * l = this->mapfile->getLayers().at(i.row());
  if (l->getType() == "MS_LAYER_RASTER") {
    this->layerSettingsDialog->setWindowTitle(tr("Raster Layer Settings"));
    LayerSettingsRaster * layerSettingsRaster = new LayerSettingsRaster(this->layerSettingsDialog, this->mapfile, l);
    mainLayout->addWidget(layerSettingsRaster);
    mainLayout->addWidget(buttonBox);
    this->layerSettingsDialog->setLayout(mainLayout);
    this->layerSettingsDialog->resize(870, 630);
    this->layerSettingsDialog->show();
  } else {
    this->layerSettingsDialog->setWindowTitle(tr("Vector Layer Settings"));
    LayerSettingsVector * layerSettingsVector = new LayerSettingsVector(this->layerSettingsDialog, this->mapfile, l);
    mainLayout->addWidget(layerSettingsVector);
    mainLayout->addWidget(buttonBox);
    this->layerSettingsDialog->setLayout(mainLayout);
    this->layerSettingsDialog->resize(870, 630);
    this->layerSettingsDialog->show();
  }
}

void MainWindow::showAbout() {
    QDialog* aboutDialog = new QDialog (this);
    aboutDialog->setWindowFlags( Qt::WindowCancelButtonHint | Qt::WindowSystemMenuHint | Qt::Dialog | Qt::WindowCloseButtonHint );
    aboutDialog->setWindowTitle(tr("About"));
    aboutDialog->setFixedSize(400,300);
    aboutDialog->setModal(true);
    QGridLayout* layout = new QGridLayout (aboutDialog);
    QLabel *content = new QLabel("<h1><b>About</b></h1><h2>Author:</h2><ul><li>Yves Jacolin</li><li>Pierre Mauduit</li></ul><h2>Licence:<h2><h2>Links:</h2><ul><li><a href='http://mapserver.org'>MapServer Project</a></li><li><a href=''>Sheetah</a></li></ul>");
    content->setTextFormat(Qt::RichText);
    //content->setText(tr("<h1><b>About</b></h1><h2>Author:</h2><h2>Licence:<h2><h2>Links:</h2>"));
    layout->addWidget(content, 0, 0);
    aboutDialog->setLayout(layout);
    aboutDialog->show();
}

void MainWindow::saveMapfile()
{
  if (this->mapfile) {
    // if this is a new mapfile, calls saveAsMapfile instead
    if (this->mapfile->isNew()) {
      this->saveAsMapfile();
    } else {
      this->mapfile->saveMapfile(QString());
    }
    //we know the mapfile filename:
    this->mapfile->saveMapfile(this->mapfiledir.path()+ "/" + this->mapfilename);
    ui->actionSave->setEnabled(false);
  }
}

void MainWindow::saveAsMapfile()
{
  QString prevFilePath = QDir::currentPath();

  if (this->mapfile) {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save map File"), prevFilePath, tr("Map file (*.map)")); 
    if (fileName.isEmpty()) {
      return;
    }
    if (! this->mapfile->saveMapfile(fileName)) {
      QMessageBox::critical(this, "QMapfileEditor", tr("Error occured while saving the mapfile."));
    }
    return;
  }
}

void MainWindow::showInfo(const QString & message)
{
   if (ui->statusbar->isEnabled() )
     ui->statusbar->showMessage(message);
   return;
}

MainWindow::~MainWindow()
{
  if (this->mapfile) {
    delete this->mapfile;
  }

  if (this->settings) {
    delete this->settings;
  }
  if (this->layerSettingsDialog) {
    delete this->layerSettingsDialog;
  }

  delete ui;
}
