#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H

#include <QDialog>

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
      QStringList units;

};

#endif // MAPSETTINGS_H

