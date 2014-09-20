#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QMainWindow>
#include <QDir>
#include <QFileDialog>

#include "mapfileparser.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void  openMapfile();
    void  newMapfile();
    void  showMapSettings();


private:
    Ui::MainWindow *ui;
    MapfileParser * mapfile = NULL;

    QStandardItemModel * mfStructureModel = NULL;
    QStandardItem * mapParamsItem = NULL;
    QStandardItem * layersItem = NULL;


    // some internals
    void reinitMapfile();
};

#endif // MAINWINDOW_H
