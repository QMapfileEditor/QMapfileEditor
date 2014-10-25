#ifndef LAYER_H
#define LAYER_H

#include <QHash>
#include <QModelIndex>
#include <QString>


/**
 * This class shall be considered as an interface
 * between the mapfile and the layers model to fit
 * in the GUI.
 */

class Layer {
  public:
    Layer(QString const & name, struct mapObj *);

    QString const & getName();
    void setName(QString const &);

  private:
    // Note: in Mapserver, name is used as a primary key
    QString name;

    struct mapObj * map;
    int getInternalIndex();
    struct layerObj * getInternalLayerObj();

};


#endif // LAYER_H

