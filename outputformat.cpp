#include "outputformat.h"


OutputFormat::OutputFormat(const QString & name,
                const QString &mimeType,
                const QString & driver,
                const QString & extension,
                const int  & imageMode,
                const bool & transparent) :
name(name),
mimeType(mimeType),
driver(driver),
extension(extension),
imageMode(imageMode),
transparent(transparent) {}

QString & OutputFormat::getName()        { return this->name;        }
QString & OutputFormat::getMimeType()    { return this->mimeType;    }
QString & OutputFormat::getDriver()      { return this->driver;      }
QString & OutputFormat::getExtension()   { return this->extension;   }
int     & OutputFormat::getImageMode()   { return this->imageMode;   }
bool    & OutputFormat::getTransparent() { return this->transparent; }

void OutputFormat::setName(const QString &v)      { this->name        = v; }
void OutputFormat::setMimeType(const QString &v)  { this->mimeType    = v; }
void OutputFormat::setDriver(const QString &v)    { this->driver      = v; }
void OutputFormat::setExtension(const QString &v) { this->extension   = v; }
void OutputFormat::setImageMode(const   int &v)   { this->imageMode   = v; }
void OutputFormat::setTransparent(const bool &v)  { this->transparent = v; }

