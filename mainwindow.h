#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

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

private:
    Ui::MainWindow *ui;
    App::MapfileParser * mapfile;
};

#endif // MAINWINDOW_H
