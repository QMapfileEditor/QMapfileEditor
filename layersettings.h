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
#include "parser/layer.h"


class LayerSettings : public QTabWidget {

  Q_OBJECT ;

 protected:
  LayerSettings(QWidget *parent, MapfileParser *, Layer *);
  MapfileParser * mapfile;
  Layer *layer;


};


#endif // LAYERSETTINGS_H
