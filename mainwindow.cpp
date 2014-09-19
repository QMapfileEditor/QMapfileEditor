#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

// private

void MainWindow::reinitMapfile() {
      this->layersItem->removeRows(0, layersItem->rowCount());
      this->mapParamsItem->removeRows(0, mapParamsItem->rowCount());
      delete this->mapfile;
      this->mapfile = NULL;
}

// public
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mfStructureModel = new QStandardItemModel();
    mapParamsItem = new QStandardItem(QString("Map parameters"));
    layersItem = new QStandardItem(QString("Layers"));
    mfStructureModel->appendRow(mapParamsItem);
    mfStructureModel->appendRow(layersItem);
    ui->mf_structure->setModel(mfStructureModel);
    this->connect(ui->actionNew, SIGNAL(triggered()), SLOT(newMapfile()));
    this->connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openMapfile()));
}

void MainWindow::newMapfile()
{
    // check if a mapfile is already opened
    if (this->mapfile->isLoaded()) {
      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Currently editing an existing mapfile", "Discard current modifications ?",
          QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
       this->reinitMapfile();
      } else {
        return;
      }
    }
}



void MainWindow::openMapfile()
{
    QString prevFilePath = QDir::homePath();
    QString fileName ;

    // Reinit / free objects if necessary
    if (this->mapfile) {
      this->reinitMapfile();
    }

    fileName = QFileDialog::getOpenFileName(this, tr("Open map File"), prevFilePath, tr("Map file (*.map)"));
    this->mapfile = new MapfileParser(fileName.toStdString());



    if (! this->mapfile->isLoaded()) {
      QMessageBox::critical(
          this,
          "QMapfileEditor",
          tr("Error occured while loading the mapfile.")
      );
      delete this->mapfile;
      return;
    }

    //TODO: move this outside
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


MainWindow::~MainWindow()
{
    if (this->mapfile) {
        delete this->mapfile;
    }
    delete this->mfStructureModel;
    // This *should* destroy the children objects
    delete ui;
}
