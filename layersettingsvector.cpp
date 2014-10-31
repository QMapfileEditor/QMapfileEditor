#include "layersettingsvector.h"
#include "ui_layersettingsvector.h"


LayerSettingsVector::LayerSettingsVector(QWidget * parent, MapfileParser * map):
  QTabWidget(parent), ui(new Ui::LayerSettingsVector), mapfile(map) {

    ui->setupUi(this);
}


LayerSettingsVector::~LayerSettingsVector() {
  delete ui;
}


void LayerSettingsVector::accept() {
  // ...
}



