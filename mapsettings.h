#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H


#include <QColorDialog>
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>

#include "mapfileparser.h"

#include "commands/changemapnamecommand.h"

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
      void accept();
      void addConfigOptionsToModel(const QString &, const QString &);
      void addNewOgcMetadata();
      void angleSliderChanged(int);
      void angleSpinChanged(int);
      void enableDebugBox(bool);
      void enableOgcStandardFrame(bool);
      void browseDebugFile();
      void browseEncryptionFile();
      void browseFontsetFile();
      void browseProjlibFile();
      void browseShapepath();
      void browseSymbolsetFile();
      void changeMapName();
      void refreshGdalOgrDriverCombo(const QString &);
      void refreshOutputFormatTab(const QModelIndex &);
      void setImageColor();

 private:
      Ui::MapSettings * ui;

      MapfileParser * mapfile;

      QUndoStack * settingsUndoStack;

      bool alreadyInModel(const QString &);
      void createOgcOptionsModel();
      void saveMapSettings();
      void toggleOutputFormatsWidgets(const bool &);

};

#endif // MAPSETTINGS_H

