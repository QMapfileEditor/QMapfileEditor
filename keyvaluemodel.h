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

#ifndef KEYVALUEMODEL_H
#define KEYVALUEMODEL_H

#include <QAbstractTableModel>
#include <QHash>
#include <QString>

/*
 * Simple model to map Key/Value stores.
 */
class KeyValueModel : public QAbstractTableModel {

  public:
    KeyValueModel(QObject *parent = 0, QStringList extra_filters = QStringList());
    ~KeyValueModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void setData(QHash<QString, QString> const &);
    void addData(QString const &, QString const &);
    void removeData(QString const &);
    void removeDataAt(QModelIndexList const &);
    QHash<QString, QString> const & getData() const;

    void setExtraFilters(QStringList const &);

    enum Column { Key, Value };
  protected:
    QHash<QString, QString>  m_data;
    QStringList extra_filters;

};

#endif // KEYVALUEMODEL_H

