#include "layersettings.h"
#include "ui_layersettings.h"

LayerSettings::LayerSettings(QWidget * parent, MapfileParser * mf) :
  QTabWidget(parent), ui(new Ui::LayerSettings), mapfile(mf)
{
    ui->setupUi(this);
    this->settingsUndoStack = new QUndoStack(this);

}


//SLOTS
void LayerSettings::accept() {
    //this->saveLayerSettings();

    //QDialog::accept();
}
/** End SLOTS **/

LayerSettings::~LayerSettings() {
  delete ui;
}
