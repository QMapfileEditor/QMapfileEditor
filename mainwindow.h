#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QDir>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QMessageBox>
#include <QPixmap>
#include <QStandardItem>
#include <QStandardItemModel>

#include "mapsettings.h"

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
      void  saveMapfile();
      void  saveAsMapfile();
      void  showMapSettings();
      void  showAbout();
      void  showInfo( QString message = NULL);

 private:
      Ui::MainWindow *ui;

      MapfileParser * mapfile = NULL;

      // Map structure (layers)
      QStandardItemModel * mfStructureModel = NULL;
      QStandardItem * layersItem = NULL;

      MapSettings * settings = NULL;

      // Objects related to the map preview
      QGraphicsScene * mapScene = NULL;


      // some internal methods
      void reinitMapfile();
};

#endif // MAINWINDOW_H
