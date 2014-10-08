#ifndef __OUTPUTFORMAT_H
#define __OUTPUTFORMAT_H

#include <QString>
#include <QHash>

class OutputFormat {
  private:
   QString name;
   QString mimeType;

   QString driver;
   QString extension;

   int imageMode;

   bool transparent;
   enum state { ADDED, REMOVED, MODIFIED, UNCHANGED };

   QHash<QString,QString> formatOptions;

  public:
   OutputFormat(const QString & name = 0,
                const QString & mimeType = 0,
                const QString & driver = 0,
                const QString & extension = 0,
                const int  & imageMode = 0,
                const bool & transparent = 0);

   /** getters */
   QString                  getName();
   QString                  getMimeType();
   QString                  getDriver();
   QString                  getExtension();
   int                      getImageMode();
   bool                     getTransparent();
   QHash<QString, QString>  getFormatOptions();
   state                    getState();

   void addFormatOption(const QString &k, const QString &v);
   void removeFormatOption(const QString &k);

   bool isEmpty();

   /** setters */
   void setName(const QString &) const;
   void setMimeType(const QString &) const;
   void setDriver(const QString &) const;
   void setExtension(const QString &) const;
   void setImageMode(const int &) const;
   void setTransparent(const bool &) const;
   void setState(const state &) const;
};

// The class defining a model to wire onto the Qt interface
class OutputFormatsModel : public QAbstractListModel {

  enum Column {
      Name, MimeType, Driver, Extension, ImageMode, Transparent
  }

  public:
    OutputFormatsModel(QObject *parent = 0): QAbstractListModel(parent) {}
    int rowCount(const QModelIndex &parent = QModelIndex()) const { Q_UNUSED(parent); return m_entries.count(); }
    int columnCount(const QModelIndex &parent = QModelIndex()) const { Q_UNUSED(parent); return ColMax; }
   
    QVariant data(const QModelIndex &index, int role) const { return QVariant(); }

  protected:
      QList<OutputFormat> m_entries;


}

#endif // __OUTPUTFORMAT_H
