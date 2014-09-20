#include "mapsettings.h"


MapSettings::MapSettings(QWidget *parent, MapfileParser *mf) :
  QDialog(parent),
  mapfile(mf) {}

MapSettings::~MapSettings() {

}
