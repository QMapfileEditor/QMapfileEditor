#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  this->showInfo(tr("Initializing default mapfile"));

  // inits the model for the mapfile structure
  QStandardItemModel * mfStructureModel = new QStandardItemModel(ui->mf_structure);
  ui->mf_structure->setModel(mfStructureModel);

  // inits the graphics scene
  QGraphicsScene * mapScene = new QGraphicsScene(ui->mf_preview);
  ui->mf_preview->setScene(mapScene);

  // connects extra actions
  this->showInfo("Activate actions");
  this->connect(ui->actionNew, SIGNAL(triggered()), SLOT(newMapfile()));
  this->connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openMapfile()));
  this->connect(ui->actionSave, SIGNAL(triggered()), SLOT(saveMapfile()));
  this->connect(ui->actionSaveAs, SIGNAL(triggered()), SLOT(saveAsMapfile()));
  this->connect(ui->actionMapSetting, SIGNAL(triggered()), SLOT(showMapSettings()));
  this->connect(ui->actionAbout, SIGNAL(triggered()), SLOT(showAbout()));
  this->connect(ui->actionRefresh, SIGNAL(triggered()), SLOT(updateMapPreview()));

  //creates a default empty mapfileparser
  this->mapfile = new MapfileParser(QString());
  this->showInfo(tr("Initialisation process: success !"));
}

void MainWindow::reinitMapfile() {
  // if a MapSettings window has been opened, closes and destroys it
  if (this->settings) {
    this->settings->close();
    delete this->settings;
    this->settings = NULL;
  }

  QStandardItemModel * mod = (QStandardItemModel *) ui->mf_structure->model();
  mod->clear();
  // Creates a new mapfileparser from scratch
  delete this->mapfile;
  this->mapfile = new MapfileParser(QString());
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
  if ((this->mapfile) &&  (this->mapfile->isLoaded())) {
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
  QString prevFilePath = QDir::homePath();

  // TODO: check if current session has unsaved modifications instead
  if (this->mapfile->isLoaded()) {
    if (warnIfActiveSession() == QMessageBox::No)
      return;
  }

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
    this->showInfo(tr("Mapfile openend with success."));
    return;
  }

  QStringList layers = this->mapfile->getLayers();
  for (int i = 0; i < layers.size(); ++i) {
    QStandardItem * si = new QStandardItem(layers.at(i));
    si->setEditable(false);
    ((QStandardItemModel *) this->ui->mf_structure->model())->appendRow(si);
  }

  ui->mf_structure->expandAll();

  this->updateMapPreview();
}

void MainWindow::updateMapPreview(void) {
  int w = this->ui->mf_preview->viewport()->width(), h = this->ui->mf_preview->viewport()->height();
  this->updateMapPreview(w, h);
}

void MainWindow::updateMapPreview(const int & w, const int &h) {
  // re-init map preview
  this->ui->mf_preview->scene()->clear();
  // rendering the map
  QPixmap mapRepr = QPixmap();
  // it is mapfileparser's class role to manage the allocated
  // memory
  unsigned char * mapImage = this->mapfile->getCurrentMapImage(w, h);
  int mapImageSize = this->mapfile->getCurrentMapImageSize();
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
  // a window has alrady been created
  if (this->settings) {
    this->settings->show();
    return;
  }
  this->settings = new MapSettings(this, this->mapfile);
  this->settings->show();
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

void MainWindow::showInfo(const QString & message)
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

  if (this->settings) {
    delete this->settings;
  }

  delete ui;
}
