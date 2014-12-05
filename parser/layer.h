/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: 
 * Author: Pierre Mauduit
 *
 **********************************************************************
 * Copyright (c) 2014, Pierre Mauduit
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of this Software or works derived from this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 ****************************************************************************/

#ifndef LAYER_H
#define LAYER_H

#include <QHash>
#include <QModelIndex>
#include <QString>
#include <QStringList>

/**
 * This class shall be considered as an interface
 * between the mapfile and the layers model to fit
 * in the GUI.
 */

class Layer {
  public:
    Layer(QString const & name, struct mapObj *);

    QString const & getName() const;
    void setName(QString const &);

    // static variables (from mapserver.h)
    static QStringList layerType;

    // Accessors
    int getStatus() const;
    QString getRequires() const;
    QString getGroup() const;
    QString getType() const;
    int getOpacity() const;
    QString getMask() const;
    QString getUnits() const;
    QString getSizeUnits() const;
    double getMinX() const;
    double getMaxX() const;
    double getMinY() const;
    double getMaxY() const;
    double getMinScale() const;
    double getMaxScale() const;
    QString getPlugin() const;
    double getTolerance() const;
    QString getToleranceUnits() const;
    int getMaxFeatures() const;
    double getMinGeoWidth() const;
    double getMaxGeoWidth() const;
    QString getClassGroup() const;
    QString getTemplate() const;
    QString getHeader() const;
    QString getFooter() const;
    QString getStyleItem() const;
    QString getFilterItem() const;
    QString getLabelItem() const;
    QString getClassItem() const;
    double getSymbolScaleDenom() const;
    bool getLabelCache() const;
    bool getPostLabelCache() const;
    int getDebugLevel() const;
    QString getLabelRequires() const;

    double getMaxScaleDenomLabel() const;
    double getMinScaleDenomLabel() const;


  private:
    // Note: in Mapserver, name is used as a primary key
    // Hence, this member variable has a specific role.
    QString name;

    // Since modifications of layers could have an impact on the in-memory
    // objects (e.g. deletion), we prefer keeping a reference the the map
    // object, instead of the layerObj ones.
    //
    // It should anyway always be possible to get back a reference to the
    // internal layer object using the mapObj.
    struct mapObj * map;

    int getInternalIndex() const;
    struct layerObj * getInternalLayerObj() const;

};

// TODO: Don't know yet if it would be of interest to introduce class hierarchy

class VectorLayer : public Layer {

};

class RasterLayer : public Layer {

};




class LayerModel : public QAbstractListModel {

 public:
  LayerModel(QObject *, QList<Layer *> const &);
  ~LayerModel();

  void setLayers(QList<Layer *> const &);
  Layer * getLayer(const QModelIndex &) const;
  void removeLayer(const QModelIndex &);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;

  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

  // TODO to be completed as long as new member variables are supported
  enum Column { LAYER_NAME, LAYER_STATUS, LAYER_TYPE, LAYER_OPACITY, LAYER_MASK, LAYER_MIN_X, LAYER_MAX_X, 
    LAYER_MIN_Y, LAYER_MAX_Y, LAYER_MIN_SCALE, LAYER_MAX_SCALE,
    LAYER_TOLERANCE, LAYER_MAX_FEATURES, LAYER_MIN_GEO_WIDTH, LAYER_MAX_GEO_WIDTH, LAYER_HEADER, LAYER_FOOTER, LAYER_LABEL_ITEM, 
    LAYER_SYMBOL_SCALE_DENOM, LAYER_MAX_SCALE_DENOM_LABEL,
    LAYER_MIN_SCALE_DENOM_LABEL, LAYER_LABEL_CACHE, LAYER_POST_LABEL_CACHE, LAYER_LABEL_REQUIRES, LAYER_DEBUG_LEVEL };

 private:
  QList<Layer *> layers;
  QList<Layer *> removedLayers;

};

#endif // LAYER_H

