#include "outputformat.h"


OutputFormat::OutputFormat(const QString & name,
                const QString &mimeType,
                const QString & driver,
                const QString & extension,
                const int  & imageMode,
                const bool & transparent,
                const enum State & state) :
name(name),
mimeType(mimeType),
driver(driver),
extension(extension),
imageMode(imageMode),
transparent(transparent),
state(state) {}

QString const & OutputFormat::getName()        const { return name;        }
QString const & OutputFormat::getMimeType()    const { return mimeType;    }
QString const & OutputFormat::getDriver()      const { return driver;      }
QString const & OutputFormat::getExtension()   const { return extension;   }
int     const & OutputFormat::getImageMode()   const { return imageMode;   }
bool    const & OutputFormat::getTransparent() const { return transparent; }

QHash<QString, QString> const & OutputFormat::getFormatOptions() const { return formatOptions; }

bool OutputFormat::isEmpty() { return name.isEmpty(); };

void OutputFormat::addFormatOption(const QString &k, const QString &v) {
   formatOptions.insert(k,v);
}

void OutputFormat::removeFormatOption(const QString &k) {
  formatOptions.remove(k);
}

void OutputFormat::setName(QString const &v)      { name        = v; }
void OutputFormat::setMimeType(QString const &v)  { mimeType    = v; }
void OutputFormat::setDriver(QString const &v)    { driver      = v; }
void OutputFormat::setExtension(QString const &v) { extension   = v; }
void OutputFormat::setImageMode(int const &v)     { imageMode   = v; }
void OutputFormat::setTransparent(bool const &v)  { transparent = v; }

/** related to OutputFormat Model (representation into the UI) */

OutputFormatsModel::OutputFormatsModel(QObject * parent) : QAbstractListModel(parent) {}

OutputFormatsModel::~OutputFormatsModel() {
  for (int i = 0; i < entries.count(); ++i) {
    delete entries.at(i);
  }
}

int OutputFormatsModel::rowCount(const QModelIndex & parent) const {
  Q_UNUSED(parent);
  return entries.count();
}

int OutputFormatsModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return OutputFormatsModel::Transparent + 1;
}

QVariant headerData (int section, Qt::Orientation orientation, int role) {
  Q_UNUSED(section);
  Q_UNUSED(orientation);
  Q_UNUSED(role);
  return QVariant("Output formats");
}

void OutputFormatsModel::setData(QList<OutputFormat *> const & items) {
  entries = items;
}

QVariant OutputFormatsModel::data(const QModelIndex &index, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();
  if (index.row() > entries.size())
    return QVariant();

  OutputFormat * of = entries.at(index.row());

  if (of == NULL)
    return QVariant();

  switch (index.column()) {
    case OutputFormatsModel::Name:
      return QVariant(of->getName());
    case OutputFormatsModel::MimeType:
      return QVariant(of->getMimeType());
    case OutputFormatsModel::Driver:
      return QVariant(of->getDriver());
    case OutputFormatsModel::Extension:
      return QVariant(of->getExtension());
    case OutputFormatsModel::ImageMode:
      return QVariant(of->getImageMode());
    case OutputFormatsModel::Transparent:
      return QVariant(of->getTransparent());
  }
  return QVariant();
}
