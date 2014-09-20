#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H

#include <QDialog>

#include "mapfileparser.h"


namespace Ui {
  class Ui_MapSettings;
}

class MapSettings : public QDialog
{
  Q_OBJECT

  public:
    explicit MapSettings(QWidget * parent, MapfileParser *);
    ~MapSettings();

  private:
    MapfileParser * mapfile;

};

#endif // MAPSETTINGS_H

