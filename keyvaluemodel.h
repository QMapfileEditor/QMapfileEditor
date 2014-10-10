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
    KeyValueModel(QObject *parent = 0);
    ~KeyValueModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void setData(QHash<QString, QString> const &);
    void addData(QString const &, QString const &);
    void removeData(QString const &);
    QHash<QString, QString> const & getData() const;

    enum Column { Key, Value };
  protected:
    QHash<QString, QString>  m_data;

};

#endif // KEYVALUEMODEL_H

