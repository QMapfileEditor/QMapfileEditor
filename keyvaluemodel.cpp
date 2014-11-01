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

#include "keyvaluemodel.h"


KeyValueModel::KeyValueModel(QObject * parent, QStringList extraFilters):
    QAbstractTableModel(parent),
    m_data(QHash<QString,QString>()),
    extra_filters(extraFilters) {
}

KeyValueModel::~KeyValueModel() {}

int KeyValueModel::rowCount(const QModelIndex & parent) const {
  Q_UNUSED(parent);
  return m_data.keys().size();
}

int KeyValueModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return KeyValueModel::Value + 1;
}

QVariant KeyValueModel::headerData (int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Vertical)
    return QVariant(section);

  if (section == KeyValueModel::Key)
    return QVariant(QObject::tr("Key"));

  else if (section == KeyValueModel::Value)
    return QVariant(QObject::tr("Value"));

  return QVariant(section);
}

void KeyValueModel::setData(QHash<QString,QString> const & kv) {
  QHash<QString,QString> cop = QHash<QString,QString>(kv);
  for (int i = 0 ; i < extra_filters.size(); ++i)
    cop.remove(extra_filters.at(i));

  beginResetModel();
  m_data = cop;
  endResetModel();
}

QHash<QString,QString> const & KeyValueModel::getData(void) const {
  return m_data;
}

void KeyValueModel::addData(QString const & k, QString const &v) {
  if (extra_filters.contains(k))
    return;
  // It might appear overkill to reset entirely the model
  // but the interface is not meant to contain hundreds of rows.
  beginResetModel();
  m_data.insert(k,v);
  endResetModel();
}

void KeyValueModel::removeData(QString const &k) {
  beginResetModel();
  m_data.remove(k);
  endResetModel();
}


void KeyValueModel::removeDataAt(QModelIndexList const & selection) {
  QStringList toRemove = QStringList();
  for (int i = 0; i < selection.size(); ++i) {
    toRemove << m_data.keys().at(selection.at(i).row());
  }
  // Doing in 2 passes to avoid messing up with indices offsets
  beginResetModel();
  for (int i = 0; i < toRemove.size(); ++i) {
    m_data.remove(toRemove.at(i));
  }
  endResetModel();

}

QVariant KeyValueModel::data(const QModelIndex &index, int role) const {

  if (! index.isValid()) {
    return QVariant();
  }

  if (role != Qt::DisplayRole)
    return QVariant();

  if (index.row() >= m_data.keys().size())
    return QVariant();

  if (index.column() > KeyValueModel::Value)
    return QVariant();

  QString key = m_data.keys().at(index.row());

  if (index.column() == KeyValueModel::Key) {
    return key;
  }
  else if (index.column() == KeyValueModel::Value) {
    return m_data.value(key);
  }

  return QVariant();
}


