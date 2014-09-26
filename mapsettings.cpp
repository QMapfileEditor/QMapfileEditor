#include "mapsettings.h"
#include "ui_mapsettings.h"

MapSettings::MapSettings(QWidget *parent, MapfileParser *mf) :
    QDialog(parent), ui(new Ui::MapSettings), mapfile(mf) {
      ui->setupUi(this); 
      this->mapfile = mf;
      
      /** Constants **/
      this->units << "inches" << "feet" << "miles" << "meters" << "kilometers" << "dd" << "pixels" << "pourcentages" << "nauticalmiles";
      this->imageTypes << "jpeg" << "pdf" << "png" << "svg";
      this->missingdata << "" << "FAIL" << "LOG" << "IGNORE";

      //TODO: create Slots and Signal on extent auto/manual to enabling forms
      
      /** General Tab **/
      //Name
      ui->mf_map_name->setText(this->mapfile->getMapName());
      //Status
      if( this->mapfile->getMapStatus() )
      {
        ui->mf_map_status_off->setChecked(false);  
	    ui->mf_map_status_on->setChecked(true);
      } else {
        ui->mf_map_status_off->setChecked(true);
        ui->mf_map_status_on->setChecked(false);
      }

      //MapSizes
      ui->mf_map_size_width->setValue(this->mapfile->getMapWidth());
      ui->mf_map_size_height->setValue(this->mapfile->getMapHeight());
      ui->mf_map_maxsize->setValue(this->mapfile->getMapSize());
      //Units
      ui->mf_map_units->addItems(this->units);
      ui->mf_map_units->setCurrentIndex(this->mapfile->getMapUnits());
      
      //Outpuformat
      ui->mf_map_outputformat->addItems(this->imageTypes);
      //TODO: add custom outputformat
      //ui->mf_map_outputformat->setCurrentIndex(this->mapfile->getMapImageTypes());
      
      //Projection
      //TODO: create autocompleter for projection
      //QCompleter *epsgCompleter = new QCompleter(, this);
      ui->mf_map_projection->addItem(QString::number(this->mapfile->getMapProjection()));
      
      //Extent
      ui->mf_map_extent_top->setText(QString::number(this->mapfile->getMapExtentMaxY()));
      ui->mf_map_extent_bottom->setText(QString::number(this->mapfile->getMapExtentMinY()));
      ui->mf_map_extent_right->setText(QString::number(this->mapfile->getMapExtentMaxX()));
      ui->mf_map_extent_left->setText(QString::number(this->mapfile->getMapExtentMinX()));

      /** Path tab **/
      ui->mf_map_shapepath->setText(this->mapfile->getShapepath());
      ui->mf_map_symbolset->setText(this->mapfile->getSymbolSet());
      ui->mf_map_fontset->setText(this->mapfile->getFontSet());
      
      /** Advanced tab **/
      //TODO: add action/slot for encryption key and proj lib path
      ui->mf_map_resolution->setValue(this->mapfile->getResolution());
      ui->mf_map_defresolution->setValue(this->mapfile->getDefResolution());
      ui->mf_map_angle->setValue(this->mapfile->getAngle());
      ui->mf_map_templatepattern->setText(this->mapfile->getTemplatePattern());
      ui->mf_map_datapattern->setText(this->mapfile->getDataPattern());
      ui->mf_map_config_contexturl->setText(this->mapfile->getConfigContextUrl());
      ui->mf_map_config_encryption->setText(this->mapfile->getConfigEncryptionKey());

      if (this->mapfile->getConfigNonsquare() != NULL ) {
          ui->mf_map_config_squarepixel_yes->setChecked(true);
          ui->mf_map_config_squarepixel_off->setChecked(false);
      } else {
          ui->mf_map_config_squarepixel_yes->setChecked(false);
          ui->mf_map_config_squarepixel_off->setChecked(true);
      }

      ui->mf_map_config_projlib->setText(this->mapfile->getConfigProjLib());

      /** Debug tab **/
      //TODO: add slot/action to enable mf_map_debug box
      //TODO: add slot/connect for browsing
      this->connect(ui->mf_map_shapepath_browse, SIGNAL(clicked()), SLOT(browseShapepath()));
      this->connect(ui->mf_map_fontset_browse, SIGNAL(clicked()), SLOT(browseFontsetFile()));
      this->connect(ui->mf_map_symbolset_browse, SIGNAL(clicked()), SLOT(browseSymbolsetFile()));

      if( this->mapfile->getDebug() )
      {
          ui->mf_map_debug_off->setChecked(false);
          ui->mf_map_debug_on->setChecked(true);
          ui->mf_map_debug->setValue(this->mapfile->getDebug());
          ui->mf_map_debug->setEnabled(true);
      } else {
          ui->mf_map_debug_off->setChecked(true);
          ui->mf_map_debug_off->setChecked(false);
          ui->mf_map_debug->setValue(this->mapfile->debug);
          ui->mf_map_debug->setEnabled(false);
      }
      //TODO: add option for relativ/absolute debug file in forms.
      //TODO: Test if file exist or form is empty, if not warn user
      this->connect(ui->mf_map_config_errorFile_browse, SIGNAL(clicked()), SLOT(browseDebugFile()));
      ui->mf_map_config_errorFile->setText(this->mapfile->getDebugFile());

      ui->mf_map_config_missingdata->addItems(this->missingdata);
      if (this->mapfile->getConfigMissingData() != NULL ) {
          //TODO: select correct item in the combobox
          //ui->mf_map_config_missingdata->setCurrentIndex(2);
          //ui->mf_map_config_missingdata->setCurrentIndex(this->missingdata.lastIndexOf("LOG"));
          ui->mf_map_config_missingdata->setCurrentIndex(this->missingdata.lastIndexOf(this->mapfile->getConfigMissingData()));
      }

}

/** Following method should be refactored **/
void MapSettings::browseDebugFile() {

      //TODO: should be defaulted to dirname of mapfile if option relativepath is set to on
      QString prevFilePath = QDir::homePath();

      QString fileName = QFileDialog::getOpenFileName(this, tr("Open debug File"), prevFilePath, tr("Debug file (*.log)"));
      // open file dialog has been discarded (escape)
      if (fileName.isEmpty()) {
            return;
      }
      //TODO: should we used relatif or absolute path? if dirname = dirname of mapfile then basename(fileName)
      ui->mf_map_config_errorFile->setText((char *) fileName.toStdString().c_str());
}

void MapSettings::browseSymbolsetFile() {

      //TODO: should be defaulted to dirname of mapfile if option relativepath is set to on
      QString prevFilePath = QDir::homePath();

      QString fileName = QFileDialog::getOpenFileName(this, tr("Open symbolset File"), prevFilePath, tr("Symbolset file (*.sym)"));
      // open file dialog has been discarded (escape)
      if (fileName.isEmpty()) {
            return;
      }
      //TODO: should we used relatif or absolute path? if dirname = dirname of mapfile then basename(fileName)
      ui->mf_map_symbolset->setText((char *) fileName.toStdString().c_str());
}

void MapSettings::browseFontsetFile() {

      //TODO: should be defaulted to dirname of mapfile if option relativepath is set to on
      QString prevFilePath = QDir::homePath();

      QString fileName = QFileDialog::getOpenFileName(this, tr("Open fontset File"), prevFilePath, tr("Fontset file (*.font)"));
      // open file dialog has been discarded (escape)
      if (fileName.isEmpty()) {
            return;
      }
      //TODO: should we used relatif or absolute path? if dirname = dirname of mapfile then basename(fileName)
      ui->mf_map_fontset->setText((char *) fileName.toStdString().c_str());
}

void MapSettings::browseShapepath() {
      //TODO: should be defaulted to dirname of mapfile if option relativepath is set to on
      QString prevDirPath = QDir::homePath();

      QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory Files"), prevDirPath);
      // open file dialog has been discarded (escape)
      if (dirName.isEmpty()) {
            return;
      }
      //TODO: should we used relatif or absolute path?
      ui->mf_map_shapepath->setText((char *) dirName.toStdString().c_str());
}
/** End refactoring **/

MapSettings::~MapSettings() {
  // mapfile lifecycle should be managed elsewhere
  delete ui;
}
