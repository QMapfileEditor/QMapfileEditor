#include "mainwindow.h"
#include "ui_mainwindow.h"

// private

void MainWindow::reinitMapfile() {
  // if a MapSettings window has been opened, closes and destroys it
  if (this->settings) {
    this->settings->close();
    delete this->settings;
    this->settings = NULL;
  }

  this->layersItem->removeRows(0, layersItem->rowCount());
  delete this->mapfile;
  this->mapfile = new MapfileParser(QString());

  // re-init map preview
  this->mapScene->clear(); 
}

// public

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // init default mapfile structure model
  this->showInfo(tr("Initializing default mapfile"));

  mfStructureModel = new QStandardItemModel();
  layersItem = new QStandardItem(tr("Layers"));
  mfStructureModel->appendRow(layersItem);
  ui->mf_structure->setModel(mfStructureModel);

  this->mapScene = new QGraphicsScene(this);
  ui->mf_preview->setScene(this->mapScene);

  // connects extra actions
  this->showInfo("Activate actions");
  this->connect(ui->actionNew, SIGNAL(triggered()), SLOT(newMapfile()));
  this->connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openMapfile()));
  this->connect(ui->actionSave, SIGNAL(triggered()), SLOT(saveMapfile()));
  this->connect(ui->actionSaveAs, SIGNAL(triggered()), SLOT(saveAsMapfile()));
  this->connect(ui->actionMapSetting, SIGNAL(triggered()), SLOT(showMapSettings()));


  //creates a default empty mapfileparser
  this->mapfile = new MapfileParser(QString());
  this->showInfo(tr("Initialisation process: success !"));
}

void MainWindow::newMapfile()
{
  // check if a mapfile is already opened
  if ((this->mapfile) &&  (this->mapfile->isLoaded())) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Currently editing an existing mapfile", "Discard current modifications ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      this->reinitMapfile();
    }
    else {
      return;
    }
  }
}

void MainWindow::openMapfile()
{
  QString prevFilePath = QDir::homePath();

  // TODO: if modifications made on a new / opened mapfile, warns the user

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open map File"), prevFilePath, tr("Map file (*.map)"));

  // open file dialog has been discarded (escape)
  if (fileName.isEmpty()) {
    return;
  }

  // Reinit / free objects if necessary
  if (this->mapfile) {
    this->reinitMapfile();
  }

  this->mapfile = new MapfileParser(fileName);

  if (! this->mapfile->isLoaded()) {
    QMessageBox::critical(
        this,
        "QMapfileEditor",
        tr("Error occured while loading the mapfile.")
        );
    this->reinitMapfile();
    this->showInfo("Mapfile openend vwith success.");
    return;
  }

  QVector<QString> * layers = this->mapfile->getLayers();
  for (int i = 0; i < layers->size(); ++i) {
    layersItem->appendRow(new  QStandardItem(layers->at(i)));
  }

  ui->mf_structure->expandAll();


  // rendering the map preview
  QPixmap mapRepr = QPixmap();
  unsigned char * mapImage = this->mapfile->getCurrentMapImage();
  int mapImageSize = this->mapfile->getCurrentMapImageSize();

  mapRepr.loadFromData(mapImage, mapImageSize);

  // TODO: use after free ?
  free(mapImage);
  this->mapScene->addPixmap(mapRepr);
}

/**
 * Displays the map settings window.
 */
void MainWindow::showMapSettings() {
  // Mapfile not loaded
  if ((! this->mapfile) || (! this->mapfile->isLoaded())) {
    return;
  }
  // a window has alrady been created
  if (this->settings) {
    this->settings->show();
    return;
  }
  this->settings = new MapSettings(this, this->mapfile);
  this->settings->show();
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
  }
}

void MainWindow::saveAsMapfile()
{
  QString prevFilePath = QDir::homePath();

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

void MainWindow::showInfo( QString message )
{
   //TODO: if statusBar is on so:
   ui->statusbar->showMessage(message);
   return;
}

MainWindow::~MainWindow()
{
  if (this->mapfile) {
    delete this->mapfile;
  }

  delete this->mfStructureModel;

  if (this->settings) {
    delete this->settings;
  }
  // TODO: since mapScene is added to an UI element,
  // is there a risk of double free here ?
  if (this->mapScene) {
    delete this->mapScene;
  }
  // This *should* destroy the children objects
  delete ui;
}
