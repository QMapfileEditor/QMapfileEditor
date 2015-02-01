#ifndef LAYERCLASSSETTINGS_H
#define LAYERCLASSSETTINGS_H

#include <QWidget>

namespace Ui {
class layerclasssettings;
}

class layerclasssettings : public QWidget
{
    Q_OBJECT

public:
    explicit layerclasssettings(QWidget *parent = 0);
    ~layerclasssettings();

private:
    Ui::layerclasssettings *ui;
};

#endif // LAYERCLASSSETTINGS_H
