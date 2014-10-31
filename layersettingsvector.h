#ifndef LAYERSETTINGSVECTOR_H
#define LAYERSETTINGSVECTOR_H

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
class LayerSettingsVector;
}

class LayerSettingsVector : public QTabWidget
{
  Q_OBJECT

 public:
      explicit LayerSettingsVector(QWidget * parent, MapfileParser *m);
      ~LayerSettingsVector();

 public slots:
      void accept();

 private:
      Ui::LayerSettingsVector * ui;

      MapfileParser * mapfile;

};

#endif // LAYERSETTINGSVECTOR_H

