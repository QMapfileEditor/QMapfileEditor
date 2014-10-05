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

   QHash<QString,QString> formatOptions;

  public:
   OutputFormat(const QString & name = 0,
                const QString & mimeType = 0,
                const QString & driver = 0,
                const QString & extension = 0,
                const int  & imageMode = 0,
                const bool & transparent = 0);

   /** getters */
   QString getName();
   QString getMimeType();
   QString getDriver();
   QString getExtension();
   int     getImageMode();
   bool    getTransparent();
   QHash<QString, QString>  getFormatOptions();


   void addFormatOption(const QString &k, const QString &v);
   void removeFormatOption(const QString &k);

   bool isEmpty();

   /** setters */
   void setName(const QString &);
   void setMimeType(const QString &);
   void setDriver(const QString &);
   void setExtension(const QString &);
   void setImageMode(const int &);
   void setTransparent(const bool &);
};


#endif // __OUTPUTFORMAT_H
