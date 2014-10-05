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

   bool isEmpty() { return name.isEmpty(); };

   /** setters */
   void setName(const QString &);
   void setMimeType(const QString &);
   void setDriver(const QString &);
   void setExtension(const QString &);
   void setImageMode(const int &);
   void setTransparent(const bool &);
};


#endif // __OUTPUTFORMAT_H
