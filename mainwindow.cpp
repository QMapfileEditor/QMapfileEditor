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
  this->mapParamsItem->removeRows(0, mapParamsItem->rowCount());
  delete this->mapfile;
  this->mapfile = new MapfileParser();

}

// public

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // init default mapfile structure model
  mfStructureModel = new QStandardItemModel();
  mapParamsItem = new QStandardItem(QString("Map parameters"));
  layersItem = new QStandardItem(QString("Layers"));
  mfStructureModel->appendRow(mapParamsItem);
  mfStructureModel->appendRow(layersItem);
  ui->mf_structure->setModel(mfStructureModel);

  // connects extra actions
  this->connect(ui->actionNew, SIGNAL(triggered()), SLOT(newMapfile()));
  this->connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openMapfile()));
  this->connect(ui->actionSave, SIGNAL(triggered()), SLOT(saveMapfile()));
  this->connect(ui->actionSaveAs, SIGNAL(triggered()), SLOT(saveAsMapfile()));
  this->connect(ui->actionMapSetting, SIGNAL(triggered()), SLOT(showMapSettings()));


  //creates a default empty mapfileparser
  this->mapfile = new MapfileParser();
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
      // Then recreates an empty mapfileparser
      this->mapfile = new MapfileParser();
    }
    else {
      return;
    }
  }
}

void MainWindow::openMapfile()
{
  QString prevFilePath = QDir::homePath();

  // TODO: if modifications made on a new / opened 
  // mapfile, warns the user

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open map File"), prevFilePath, tr("Map file (*.map)"));

  // open file dialog has been discarded (escape)
  if (fileName.isEmpty()) {
    return;
  }

  // Reinit / free objects if necessary
  if (this->mapfile) {
    this->reinitMapfile();
  }

  this->mapfile = new MapfileParser(fileName.toStdString());

  if (! this->mapfile->isLoaded()) {
    QMessageBox::critical(
        this,
        "QMapfileEditor",
        tr("Error occured while loading the mapfile.")
        );
    this->reinitMapfile();
    return;
  }

  //TODO: this should be moved outside ?

  // map title
  QList<QStandardItem *>  mapNameLst = QList<QStandardItem *>();
  // key
  mapNameLst.append(new QStandardItem(tr("name")));
  //value
  mapNameLst.append(new QStandardItem(this->mapfile->getMapName()));
  mapParamsItem->appendRow(mapNameLst);

  QVector<QString> * layers = this->mapfile->getLayers();
  for (int i = 0; i < layers->size(); ++i) {
    layersItem->appendRow(new  QStandardItem(layers->at(i)));
  }
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
        this->mapfile->saveMapfile();
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
        if (this->mapfile->saveAsMapfile(fileName.toStdString()) == -1) {
          QMessageBox::critical(this, "QMapfileEditor", tr("Error occured while saving the mapfile."));
        }
	return;
    }
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
  // This *should* destroy the children objects
  delete ui;
}
