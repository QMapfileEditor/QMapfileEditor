#include "mapsettings.h"
#include "ui_mapsettings.h"

MapSettings::MapSettings(QWidget *parent, MapfileParser *mf) :
    QDialog(parent), ui(new Ui::MapSettings), mapfile(mf) {
      ui->setupUi(this); 
      this->mapfile = mf;
      
      /** Constants **/
      this->units << "inches" << "feet" << "miles" << "meters" << "kilometers" << "dd" << "pixels" << "pourcentages" << "nauticalmiles";
      
      //TODO: create Slots and Signal on extent auto/manual to enabling forms
      
      /** General Tab **/
      //
      ui->mf_map_name->setText(this->mapfile->getMapName());
      
      if( this->mapfile->getMapStatus() )
      {
	ui->mf_map_status_off->setChecked(false);  
	ui->mf_map_status_on->setChecked(true);
      } else {
	ui->mf_map_status_off->setChecked(true);
	ui->mf_map_status_on->setChecked(false);
      }
      
      ui->mf_map_size_width->setValue(this->mapfile->getMapWidth());
      ui->mf_map_size_height->setValue(this->mapfile->getMapHeight());
      ui->mf_map_maxsize->setValue(this->mapfile->getMapSize());
      
      ui->mf_map_units->addItems(this->units);
      ui->mf_map_units->setCurrentIndex(this->mapfile->getMapUnits());
      
      ui->mf_map_extent_top->setText(QString::number(this->mapfile->getMapExtentMaxY()));
      ui->mf_map_extent_bottom->setText(QString::number(this->mapfile->getMapExtentMinY()));
      ui->mf_map_extent_right->setText(QString::number(this->mapfile->getMapExtentMaxX()));
      ui->mf_map_extent_left->setText(QString::number(this->mapfile->getMapExtentMinX()));
    }

MapSettings::~MapSettings() {
  // mapfile lifecycle should be managed elsewhere
  delete ui;
}
