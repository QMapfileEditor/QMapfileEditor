#ifndef CLASSSTYLESETTING_H
#define CLASSSTYLESETTING_H

#include <QWidget>

namespace Ui {
class classstylesetting;
}

class classstylesetting : public QWidget
{
    Q_OBJECT

public:
    explicit classstylesetting(QWidget *parent = 0);
    ~classstylesetting();

private:
    Ui::classstylesetting *ui;
};

#endif // CLASSSTYLESETTING_H
