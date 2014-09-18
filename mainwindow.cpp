#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"


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
    this->connect(ui->mf_tb_open, SIGNAL(clicked()), SLOT(openMapfile()));
}


void MainWindow::openMapfile()
{
    QString prevFilePath = QDir::homePath();
    QString fileName ;

    fileName = QFileDialog::getOpenFileName(this, tr("Open map File"), prevFilePath, tr("Map file (*.map)"));
    this->mapfile = new MapfileParser(fileName.toStdString());
    

    QVector<QString> * layers = this->mapfile->getLayers();

    if (layers == NULL) {
      QMessageBox::critical( 
          this, 
          "QMapfileEditor", 
          tr("Error occured while loading the mapfile.") );
      delete this->mapfile;
      return;
    }

    for (int i = 0; i < layers->size(); ++i) {
      layersItem->appendRow(new  QStandardItem(layers->at(i)));
    }
    std::cout << fileName.toStdString() << "\n" ;
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
