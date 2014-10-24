#ifndef LAYERSETTINGS_H
#define LAYERSETTINGS_H

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
class LayerSettings;
}

class LayerSettings : public QTabWidget
{
  Q_OBJECT

 public:
      explicit LayerSettings(QWidget * parent, MapfileParser *);
      ~LayerSettings();

 public slots:
      void accept();

 private:
      Ui::LayerSettings * ui;

      MapfileParser * mapfile;

      QUndoStack * settingsUndoStack;

      //void saveLayerSettings();

};

#endif // LAYERSETTINGS_H

