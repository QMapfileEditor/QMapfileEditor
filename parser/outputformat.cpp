#include "outputformat.h"

/** OutputFormat related */

OutputFormat::OutputFormat(const QString & name, const QString &mimeType, const QString & driver,
                const QString & extension, const int  & imageMode, const bool & transparent,
                const enum State & state) :
name(name), mimeType(mimeType), driver(driver), extension(extension),
imageMode(imageMode), transparent(transparent), state(state)
{}

/** Getters */

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
void OutputFormat::setState(enum State const &v)  { state       = v; }

/** OutputFormat Model (representation into the UI) */

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

QVariant OutputFormatsModel::headerData (int section, Qt::Orientation orientation, int role) const {
  Q_UNUSED(section);
  Q_UNUSED(orientation);
  if (role != Qt::DisplayRole)
    return QVariant();
  return QVariant(QObject::tr("Output formats"));
}

void OutputFormatsModel::setEntries(QList<OutputFormat *> const & items) {
  beginResetModel();
  entries = items;
  endResetModel();
}

QList<OutputFormat *> const & OutputFormatsModel::getEntries(void) const {
  return entries;
}

OutputFormat * OutputFormatsModel::getOutputFormat(const QModelIndex &index) const {
  if (index.row() < 0 || index.row() > entries.size())
    return NULL;
  return entries.at(index.row());
}

QVariant OutputFormatsModel::data(const QModelIndex &index, int role) const {
  if ((role != Qt::DisplayRole) && (role != Qt::EditRole))
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

bool OutputFormatsModel::nameAlreadyIn(const QString & key) {
   for (int i = 0; i < entries.size();  ++i) {
     if (entries.at(i)->getName() == key)
      return true;
   }
   return false;
}

bool OutputFormatsModel::setData(const QModelIndex & index, const QVariant & value, int role) {
 if (role != Qt::EditRole)
   return false;

 if (index.row() > entries.size())
    return false;

  OutputFormat * of = entries.at(index.row());

  if (of == NULL)
    return false;

  switch (index.column()) {
    case OutputFormatsModel::Name:
      if (nameAlreadyIn(value.toString())) {
        return false;
      }
      of->setName(value.toString());
      of->setState(OutputFormat::MODIFIED);
      return true;

    case OutputFormatsModel::MimeType:
      of->setMimeType(value.toString());
      of->setState(OutputFormat::MODIFIED);
      return true;

    case OutputFormatsModel::Driver:
      of->setDriver(value.toString());
      of->setState(OutputFormat::MODIFIED);
      return true;

    case OutputFormatsModel::Extension:
      of->setExtension(value.toString());
      of->setState(OutputFormat::MODIFIED);
      return true;

    case OutputFormatsModel::ImageMode:
      of->setImageMode(value.toInt());
      of->setState(OutputFormat::MODIFIED);
      return true;

    case OutputFormatsModel::Transparent:
      of->setTransparent(value.toBool());
      of->setState(OutputFormat::MODIFIED);
      return true;
  }
  return false;


}

