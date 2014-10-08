#ifndef __OUTPUTFORMAT_H
#define __OUTPUTFORMAT_H

#include <QHash>
#include <QModelIndex>
#include <QString>

class OutputFormat {
  public:
   enum State { ADDED, REMOVED, MODIFIED, UNCHANGED };

   OutputFormat(const QString & name = 0,
                const QString & mimeType = 0,
                const QString & driver = 0,
                const QString & extension = 0,
                const int  & imageMode = 0,
                const bool & transparent = 0,
                const enum State & state = ADDED);

   /** getters */
   QString                 const & getName()          const ;
   QString                 const & getMimeType()      const ;
   QString                 const & getDriver()        const ;
   QString                 const & getExtension()     const ;
   int                     const & getImageMode()     const ;
   bool                    const & getTransparent()   const ;
   QHash<QString, QString> const & getFormatOptions() const ;
   enum State              const & getState()         const ;

   void addFormatOption(const QString &k, const QString &v);
   void removeFormatOption(const QString &k);

   bool isEmpty();

   /** setters */
   void setName(QString const &);
   void setMimeType(QString const &);
   void setDriver(QString const &);
   void setExtension(QString const &);
   void setImageMode(int const &);
   void setTransparent(bool const &);
   void setState(enum State const &);

  private:
   QString name;
   QString mimeType;

   QString driver;
   QString extension;

   int imageMode;

   bool transparent;

   QHash<QString,QString> formatOptions;

   enum State state;


};

// The class defining a model to wire onto the Qt interface
class OutputFormatsModel : public QAbstractListModel {
  public:
    OutputFormatsModel(QObject *parent = 0);
    ~OutputFormatsModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole );

    void setData(QList<OutputFormat *> const &);

    enum Column { Name, MimeType, Driver, Extension, ImageMode, Transparent };
  protected:
      QList<OutputFormat *> entries;

};

#endif // __OUTPUTFORMAT_H
