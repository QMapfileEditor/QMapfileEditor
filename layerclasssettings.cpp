#include "layerclasssettings.h"
#include "ui_layerclasssettings.h"

layerclasssettings::layerclasssettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::layerclasssettings)
{
    ui->setupUi(this);
}

layerclasssettings::~layerclasssettings()
{
    delete ui;
}
