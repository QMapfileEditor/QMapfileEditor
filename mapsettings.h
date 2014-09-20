#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H

#include <QDialog>

#include "ui_mapsettings.h"
#include "mapfileparser.h"


namespace Ui {
  class MapSettings;
}

class MapSettings : public QDialog
{
  Q_OBJECT

  public:
    explicit MapSettings(QWidget * parent, MapfileParser *);
    ~MapSettings();

  private:
    Ui::MapSettings * ui;
    MapfileParser * mapfile;

};

#endif // MAPSETTINGS_H

