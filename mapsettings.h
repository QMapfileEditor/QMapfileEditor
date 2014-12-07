/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: Settings of Map Object
 * Author: Pierre Mauduit / Yves Jacolin
 *
 **********************************************************************
 * Copyright (c) 2014, Pierre Mauduit
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of this Software or works derived from this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 ****************************************************************************/

#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H

#include <QAbstractListModel>
#include <QColorDialog>
#include <QDataWidgetMapper>
#include <QDesktopServices>
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>

#include "keyvaluemodel.h"

#include "commands/changemapnamecommand.h"
#include "commands/changemapstatuscommand.h"
#include "commands/outputformatcommands.h"
#include "commands/setanglecommand.h"
#include "commands/setconfigoptioncommand.h"
#include "commands/setdatapatterncommand.h"
#include "commands/setdefresolutioncommand.h"
#include "commands/setimagecolorcommand.h"
#include "commands/setmapdebugcommand.h"
#include "commands/setmapextentcommand.h"
#include "commands/setmapmaxsizecommand.h"
#include "commands/setmapprojectioncommand.h"
#include "commands/setmapsizecommand.h"
#include "commands/setmapunitscommand.h"
#include "commands/setmetadatacommand.h"
#include "commands/setresolutioncommand.h"
#include "commands/setshapepathcommand.h"
//TO REMOVE #include "commands/setfontsetcommand.h"
#include "commands/setsymbolsetcommand.h"
#include "commands/settemplatepatterncommand.h"

#include "parser/mapfileparser.h"

// forward declaration
class MainWindow;

namespace Ui {
class MapSettings;
}

class MapSettings : public QDialog
{
  Q_OBJECT

 public:
      explicit MapSettings(MainWindow *, MapfileParser *);
      ~MapSettings();
      static QStringList OgcFilteredOptions;

 public slots:
      void accept();
      void addFormatOption();
      void addNewOutputFormat(void);
      void addOgcMetadata();
      void angleSliderChanged(int);
      void angleSpinChanged(int);
      void browseDebugFile();
      void browseEncryptionFile();
      //void browseFontsetFile();
      void browseProjlibFile();
      void browseShapepath();
      void browseSymbolsetFile();
      void enableDebugBox(bool);
      void enableOgcStandardFrame(bool);
      void enableRelativePathDebug();
      void enableRelativePathEncryption();
      //void enableRelativePathFontset();
      void enableRelativePathProjlib();
      void enableRelativePathShapepath();
      void enableRelativePathSymbolset();
      void handleOutputFormatFormClick(QAbstractButton *);
      void openProjectionInfo();
      void refreshGdalOgrDriverCombo(const QString &);
      void refreshOutputFormatTab(const QModelIndex &);
      void refreshOutputFormatTab(void);
      void reinitOutputFormatForm();
      void removeFormatOptions();
      void removeOgcMetadatas();
      void removeOutputFormat();
      void setImageColor();
      void switchProjectionMode();

 private:
      Ui::MapSettings * ui;

      MapfileParser * mapfile;

      QDataWidgetMapper * outputFormatsMapper;

      void populateDefaultOutputFormatList(void);
      void saveMapSettings();
      void toggleOutputFormatsWidgets(const bool &);
      QMessageBox::StandardButton warnIfActiveSession(void);
};

#endif // MAPSETTINGS_H

