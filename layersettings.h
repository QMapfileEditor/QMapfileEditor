#ifndef LAYERSETTINGS_H
#define LAYERSETTINGS_H

#include <QAbstractListModel>
#include <QColorDialog>
#include <QComboBox>
#include <QDataWidgetMapper>
#include <QDialog>
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include <QRadioButton>
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

    void initStatusRadioButton(QRadioButton *, QRadioButton *, QRadioButton *);
    void initRequiresMaskCombo(QComboBox *, QComboBox *);

    void accept();
    void reject();

    virtual QString getLayerName() const = 0;
};


#endif // LAYERSETTINGS_H
