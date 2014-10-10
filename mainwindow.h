#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QDir>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPixmap>
#include <QResizeEvent>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QUndoStack>

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
      void openMapfile();
      void newMapfile();
      void saveMapfile();
      void saveAsMapfile();
      void showMapSettings();
      void showAbout();
      void showInfo(const QString & message);
      void updateMapPreview(void);

 private:
      Ui::MainWindow *ui;

      MapfileParser * mapfile = NULL;

      // Dialog which handles the mapfile settings
      MapSettings * settings = NULL;

      QUndoStack * undoStack;

      // internal methods
      void reinitMapfile();
      void updateMapPreview(const int &, const int &);
      QMessageBox::StandardButton warnIfActiveSession(void);
};

#endif // MAINWINDOW_H
