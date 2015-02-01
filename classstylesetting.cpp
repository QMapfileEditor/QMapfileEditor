#include "classstylesetting.h"
#include "ui_classstylesetting.h"

classstylesetting::classstylesetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::classstylesetting)
{
    ui->setupUi(this);
}

classstylesetting::~classstylesetting()
{
    delete ui;
}
