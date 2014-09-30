#ifndef __OUTPUTFORMAT_H
#define __OUTPUTFORMAT_H

#include <QString>


class OutputFormat {
  private:
   QString name;
   QString mimeType;

   QString driver;
   QString extension;

   QString imageMode;

   bool transparent;

  public:
   OutputFormat(const QString & name,
                const QString &mimeType,
                const QString & driver,
                const QString & extension,
                const QString & imageMode,
                const bool & transparent);

   /** getters */
   QString & getName();
   QString & getMimeType();
   QString & getDriver();
   QString & getExtension();
   QString & getImageMode();
   bool    & getTransparent();


   /** setters */
   void setName(const QString &);
   void setMimeType(const QString &);
   void setDriver(const QString &);
   void setExtension(const QString &);
   void setImageMode(const QString &);
   void setTransparent(const bool &);
};


#endif // __OUTPUTFORMAT_H
