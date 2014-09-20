#include "mapsettings.h"
#include "ui_mapsettings.h"

MapSettings::MapSettings(QWidget *parent, MapfileParser *mf) :
  QDialog(parent), ui(new Ui::MapSettings), mapfile(mf) {
    ui->setupUi(this); 
    this->mapfile = mf;
  }

MapSettings::~MapSettings() {
  // mapfile lifecycle should be managed elsewhere
  delete ui;
}
