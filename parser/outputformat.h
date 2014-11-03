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

#ifndef OUTPUTFORMAT_H
#define OUTPUTFORMAT_H

#include <QHash>
#include <QModelIndex>
#include <QString>

class OutputFormat {
  public:

   
   /**
    * - ADDED: Marks the outputformat as "to be added to the mapfile". It as yet
    *   no existence outside the application,
    *
    * - ADDED_SAVED: The outputformat has no existence into the Mapfile yet, but
    *   has already been saved in the interface, so in further editions, it
    *   should not be removed (from the interface).
    *
    * - MODIFIED: Marks the outputformat as "needs modification when edition
    *   will finish". This means the format has already an existence
    *   mapfile-side,
    *
    * - UNCHANGED: Marks the outputformats coming from the mapfile, that have
    *   not been modified by the user yet.
    *
    */
   enum State { ADDED, ADDED_SAVED, MODIFIED, UNCHANGED };

   OutputFormat(const QString & name = 0,
                const QString & mimeType = 0,
                const QString & driver = 0,
                const QString & extension = 0,
                const int  & imageMode = 0,
                const bool & transparent = 0,
                const enum State & state = ADDED);

   /** getters */
   QString                 const & getName()          const ;
   QString                 const & getOriginalName()  const ;
   QString                 const & getMimeType()      const ;
   QString                 const & getDriver()        const ;
   QString                 const & getGdalDriver()    const ;
   QString                 const & getExtension()     const ;
   int                     const & getImageMode()     const ;
   bool                    const & getTransparent()   const ;
   QHash<QString, QString> const & getFormatOptions() const ;
   enum State              const & getState()         const ;

   void addFormatOption(const QString &k, const QString &v);
   void removeFormatOption(const QString &k);

   bool isEmpty();

   /** setters */
   void setFormatOptions(QHash<QString,QString> const &);
   void setName(QString const &);
   void setMimeType(QString const &);
   void setDriver(QString const &);
   void setExtension(QString const &);
   void setImageMode(int const &);
   void setTransparent(bool const &);
   void setState(enum State const &);

  private:
   QString name;
   // Since the key is used by Mapserver to identify
   // the outputformats, we need to keep a trace
   // here in case of the user renames it.
   QString originalName;
   QString mimeType;
   QString driver;
   QString gdalDriver;
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

    bool nameAlreadyIn(QString const &);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    void setEntries(QList<OutputFormat *> const &);
    QList<OutputFormat *> const & getEntries() const;
    QList<OutputFormat *> const & getRemovedEntries() const;

    OutputFormat * getOutputFormat(const QModelIndex & index) const;
    void removeOutputFormat(const QModelIndex & index);

    enum Column { Name, MimeType, Driver, GdalDriver, Extension, ImageMode, Transparent };

  protected:
      QList<OutputFormat *> entries;
      QList<OutputFormat *> removedEntries;

};

#endif // OUTPUTFORMAT_H
