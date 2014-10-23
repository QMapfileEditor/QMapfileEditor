#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H

#include <QAbstractListModel>
#include <QColorDialog>
#include <QDataWidgetMapper>
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>

#include "keyvaluemodel.h"
#include "commands/changemapnamecommand.h"
#include "parser/mapfileparser.h"

namespace Ui {
class MapSettings;
}

class MapSettings : public QDialog
{
  Q_OBJECT

 public:
      explicit MapSettings(QWidget * parent, MapfileParser *);
      ~MapSettings();
      static QStringList OgcFilteredOptions;

 public slots:
      void accept();
      void addFormatOption();
      void addOgcMetadata();
      void angleSliderChanged(int);
      void angleSpinChanged(int);
      void addNewOutputFormat(void);
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
      void refreshOutputFormatTab(void);
      void removeFormatOptions();
      void removeOgcMetadatas();
      void setImageColor();

 private:
      Ui::MapSettings * ui;

      MapfileParser * mapfile;

      QUndoStack * settingsUndoStack;

      QDataWidgetMapper * outputFormatsMapper;

      void saveMapSettings();
      void toggleOutputFormatsWidgets(const bool &);

};

#endif // MAPSETTINGS_H

