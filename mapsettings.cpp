#include "mapsettings.h"
#include "ui_mapsettings.h"

MapSettings::MapSettings(QWidget *parent, MapfileParser *mf) :
    QDialog(parent), ui(new Ui::MapSettings), mapfile(mf) {
      ui->setupUi(this); 
      this->mapfile = mf;
      
      /** Constants **/
      this->units << "inches" << "feet" << "miles" << "meters" << "kilometers" << "dd" << "pixels" << "pourcentages" << "nauticalmiles";
      this->imageTypes << "jpeg" << "pdf" << "png" << "svg";
      
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
      ui->mf_map_shapepath->setText(this->mapfile->shapepath);
      ui->mf_map_symbolset->setText(this->mapfile->symbolset);
      ui->mf_map_fontset->setText(this->mapfile->fontset);

      /** Debug tab **/
      //TODO: add slot/action to enable mf_map_debug box
      //TODO: add slot/connect for browsing
      if( this->mapfile->getDebugStatus() )
      {
          ui->mf_map_debug_off->setChecked(false);
          ui->mf_map_debug_on->setChecked(true);
          ui->mf_map_debug->setValue(this->mapfile->debug);
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
}

void MapSettings::browseDebugFile() {
      QString prevFilePath = QDir::homePath();

      QString fileName = QFileDialog::getOpenFileName(this, tr("Open debug File"), prevFilePath, tr("Debug file (*.log)"));
      // open file dialog has been discarded (escape)
      if (fileName.isEmpty()) {
            return;
      }
      //TODO: should we used relatif or absolute path?
      //TODO: not working
      ui->mf_map_config_errorFile->setText((char *) filename.toStdString().c_str());
}

MapSettings::~MapSettings() {
  // mapfile lifecycle should be managed elsewhere
  delete ui;
}
