#ifndef __OUTPUTFORMAT_H
#define __OUTPUTFORMAT_H

#include <QString>


class OutputFormat {
  private:
   QString name;
   QString mimeType;

   QString driver;
   QString extension;

   int imageMode;

   bool transparent;

  public:
   OutputFormat(const QString & name,
                const QString & mimeType,
                const QString & driver,
                const QString & extension,
                const int  & imageMode,
                const bool & transparent);

   /** getters */
   QString & getName();
   QString & getMimeType();
   QString & getDriver();
   QString & getExtension();
   int     & getImageMode();
   bool    & getTransparent();


   /** setters */
   void setName(const QString &);
   void setMimeType(const QString &);
   void setDriver(const QString &);
   void setExtension(const QString &);
   void setImageMode(const int &);
   void setTransparent(const bool &);
};


#endif // __OUTPUTFORMAT_H
