#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H

#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>

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
      void browseShapepath();
      void browseSymbolsetFile();
      void browseFontsetFile();
      void browseProjlibFile();
      void browseEncryptionFile();
      void angleSliderChanged(int);
      void angleSpinChanged(int);
      void enableDebugBox(bool);
      void enableOgcStandardFrame(bool);
      void addNewOgcMetadata();
      void addConfigOptionsToModel(const QString &, const QString &);

 private:
      Ui::MapSettings * ui;
      MapfileParser * mapfile;
      QStringList units;
      QStringList imageTypes;
      QStringList missingdata;
      QStringList ogcMapOptions;
      void createOgcOptionsModel();
};

#endif // MAPSETTINGS_H

