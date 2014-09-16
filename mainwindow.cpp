#include <QStandardItemModel>

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->connect(ui->mf_tb_open, SIGNAL(clicked()), SLOT(openMapfile()));
}


void MainWindow::openMapfile()
{
    QString prevFilePath = QDir::homePath();
    QString fileName ;

    fileName = QFileDialog::getOpenFileName(this, tr("Open map File"), prevFilePath, tr("Map file (*.map)"));
    this->mapfile = new MapfileParser(fileName.toStdString());

    QVector<QString> * layers = this->mapfile->getLayers();

    QStandardItemModel * layertree = new QStandardItemModel();


    for (int i = 0; i < layers->size(); ++i) {
      layertree->appendRow(new  QStandardItem(layers->at(i)));
      std::cout << "\t" << layers->at(i).toStdString()   << std::endl;
    }
    ui->mf_structure->setModel(layertree);
    std::cout << fileName.toStdString() << "\n" ;
}


MainWindow::~MainWindow()
{
// this commented out will likely cause a memory leak ...
//    if (this->mapfile) {
//        delete this->mapfile;
//    }
    delete ui;
}
