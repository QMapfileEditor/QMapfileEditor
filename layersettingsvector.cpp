#include "layersettingsvector.h"
#include "ui_layersettingsvector.h"

LayerSettingsVector::LayerSettingsVector(QWidget * parent, MapfileParser * mf) :
  QTabWidget(parent), ui(new Ui::LayerSettingsVector), mapfile(mf)
{
    ui->setupUi(this);

}


//SLOTS
void LayerSettingsVector::accept() {
    //this->saveLayerSettings();

    //QDialog::accept();
}
/** End SLOTS **/

LayerSettingsVector::~LayerSettingsVector() {
  delete ui;
}
