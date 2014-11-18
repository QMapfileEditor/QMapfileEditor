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

    QString const & getName();
    void setName(QString const &);

    // static variables (from mapserver.h)
    static QStringList layerType;

    // Accessors
    bool const & getStatus() const { return status; };
    QString const & getType() const { return type; };
    int const & getOpacity() const { return opacity; };
    QString const & getMask() const { return mask; };
    double const & getMinX() const { return minx; };
    double const & getMaxX() const { return maxx; };
    double const & getMinY() const { return miny; };
    double const & getMaxY() const { return maxy; };
    double const & getMinScale() const { return minScale; };
    double const & getMaxScale() const { return maxScale; };
    double const & getTolerance() const { return tolerance; };
    int const & getMaxFeatures() const { return maxfeatures; };
    double const & getMinGeoWidth() const { return mingeowidth; } ;
    double const & getMaxGeoWidth() const { return maxgeowidth; };
    QString const & getHeader() const { return header; };
    QString const & getFooter() const { return footer; };
    QString const & getLabelItem() const { return labelItem; };
    double const & getMaxScaleDenomLabel() const { return maxScaleDenomLabel; };
    double const & getMinScaleDenomLabel() const { return minScaleDenomLabel; };
    bool const & getLabelCache() const { return labelCache; };
    bool const & getPostLabelCache() const { return postLabelCache; };
    int const & getDebugLevel() const { return debugLevel; };
    QString const & getLabelRequires() const { return labelRequires; };


  private:
    // Note: in Mapserver, name is used as a primary key
    // Hence, this member variable has a specific role.
    QString name;

    // Since modifications of layers could have an impact on the in-memory
    // objects (e.g. deletion), we prefer keeping a reference the the map
    // object, instead of the layerObj ones.
    struct mapObj * map;

    int getInternalIndex();
    struct layerObj * getInternalLayerObj();

    // Significant member variables for the model

    /** General tab  */
    bool status;
    // TODO: there may be some types (enums ?) more appropriate
    // For now, getting what @yjacolin put in the interface
    QString type;
    QString geomType;
    int opacity;
    QString mask;
    QString group;
    QString requires;

    QString plugin;
    // TODO These could probably be merged
    QString projType;
    QString projString;

    double minx, miny, maxx, maxy;

    QString filter;

    // minscale / maxscale onto layerObj (mapserver side)
    double minScale, maxScale;


    /** Advanced tab */
    QString geomTransformation;
    double tolerance;
    QString toleranceUnit;
    QString processing;

    bool transform;
    int maxfeatures;
    double mingeowidth, maxgeowidth;

    // TODO: cluster ??


    QString layerTemplate;
    QString header, footer;

    /** classes tab */
    // TODO: needs a LayerClass class definition

    /** label tab  */
    QString labelItem;
    double minScaleDenomLabel, maxScaleDenomLabel;
    int labelAngleItem;
    bool labelCache;
    bool postLabelCache;
    int labelSizeItem;
    QString labelRequires;

    /** web services tab */

    /** validation tab */
    QString validation;

    /** debug tab */
    int debugLevel;

};

class LayerModel : public QAbstractListModel {

 public:
  LayerModel(QObject *, QList<Layer *> const &);
  ~LayerModel();

  Layer * getLayer(const QModelIndex &) const;
  void removeLayer(const QModelIndex &);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;

  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

  // TODO to be completed as long as new member variables are supported
  enum Column { LAYER_NAME, LAYER_STATUS, LAYER_TYPE, LAYER_OPACITY, LAYER_MASK, LAYER_MIN_X, LAYER_MAX_X, 
    LAYER_MIN_Y, LAYER_MAX_Y, LAYER_MIN_SCALE, LAYER_MAX_SCALE,
    LAYER_TOLERANCE, LAYER_MAX_FEATURES, LAYER_MIN_GEO_WIDTH, LAYER_MAX_GEO_WIDTH, LAYER_HEADER, LAYER_FOOTER, LAYER_LABEL_ITEM, LAYER_MAX_SCALE_DENOM_LABEL,
    LAYER_MIN_SCALE_DENOM_LABEL, LAYER_LABEL_CACHE, LAYER_POST_LABEL_CACHE, LAYER_LABEL_REQUIRES, LAYER_DEBUG_LEVEL };

 private:
  QList<Layer *> layers;
  QList<Layer *> removedLayers;

};

#endif // LAYER_H

