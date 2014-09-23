#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H

#include <QDialog>
#include <QDir>
#include <QFileDialog>

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
 public slots:
      void browseDebugFile();

 private:
      Ui::MapSettings * ui;
      MapfileParser * mapfile;
      QStringList units;
      QStringList imageTypes;
      QString filename = NULL;
};

#endif // MAPSETTINGS_H

