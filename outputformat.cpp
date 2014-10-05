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

QString OutputFormat::getName()        { return name;        }
QString OutputFormat::getMimeType()    { return mimeType;    }
QString OutputFormat::getDriver()      { return driver;      }
QString OutputFormat::getExtension()   { return extension;   }
int     OutputFormat::getImageMode()   { return imageMode;   }
bool    OutputFormat::getTransparent() { return transparent; }

bool OutputFormat::isEmpty() { return name.isEmpty(); };

void OutputFormat::setName(const QString &v)      { name        = v; }
void OutputFormat::setMimeType(const QString &v)  { mimeType    = v; }
void OutputFormat::setDriver(const QString &v)    { driver      = v; }
void OutputFormat::setExtension(const QString &v) { extension   = v; }
void OutputFormat::setImageMode(const   int &v)   { imageMode   = v; }
void OutputFormat::setTransparent(const bool &v)  { transparent = v; }

