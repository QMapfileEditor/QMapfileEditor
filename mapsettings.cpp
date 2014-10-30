#include "mainwindow.h"

#include "mapsettings.h"
#include "ui_mapsettings.h"

MapSettings::MapSettings(MainWindow * parent, MapfileParser * mf) :
  QDialog(parent), ui(new Ui::MapSettings), mapfile(mf)
{
    ui->setupUi(this);

    /** Main Tab **/

    //Name
    ui->mf_map_name->setText(this->mapfile->getMapName());
    //Status
    if( this->mapfile->getMapStatus() )
    {
      ui->mf_map_status_off->setChecked(false);
    ui->mf_map_status_on->setChecked(true);
    } else {
      ui->mf_map_status_off->setChecked(true);
      ui->mf_map_status_on->setChecked(false);
    }
    //MapSizes
    ui->mf_map_size_width->setValue(this->mapfile->getMapWidth());
    ui->mf_map_size_height->setValue(this->mapfile->getMapHeight());
    ui->mf_map_maxsize->setValue(this->mapfile->getMapMaxsize());
    //Units
    ui->mf_map_units->addItems(MapfileParser::units);
    ui->mf_map_units->setCurrentIndex(this->mapfile->getMapUnits());

    // Force extent to be numerical
    ui->mf_map_extent_left->setValidator(new QDoubleValidator(this));
    ui->mf_map_extent_bottom->setValidator(new QDoubleValidator(this));
    ui->mf_map_extent_right->setValidator(new QDoubleValidator(this));
    ui->mf_map_extent_top->setValidator(new QDoubleValidator(this));


    /** Output formats tab **/

    this->outputFormatsMapper = new QDataWidgetMapper(this);
    OutputFormatsModel * outputFormatsModel = new OutputFormatsModel(this);
    outputFormatsModel->setEntries(this->mapfile->getOutputFormats());
    ui->mf_outputformat_list->setModel(outputFormatsModel);
    for (int i = 1; i < outputFormatsModel->columnCount(); i++)
      ui->mf_outputformat_list->hideColumn(i);
    this->outputFormatsMapper->setModel(outputFormatsModel);
    this->outputFormatsMapper->addMapping(ui->mf_outputformat_name,      OutputFormatsModel::Name);
    this->outputFormatsMapper->addMapping(ui->mf_outputformat_driver,    OutputFormatsModel::Driver);
    this->outputFormatsMapper->addMapping(ui->mf_outputformat_extension, OutputFormatsModel::Extension);
    this->outputFormatsMapper->addMapping(ui->mf_outputformat_imagemode, OutputFormatsModel::ImageMode);
    this->outputFormatsMapper->addMapping(ui->mf_outputformat_mimetype,  OutputFormatsModel::MimeType);
    this->ui->mf_outputformat_formatoptions_list->setModel(new KeyValueModel(this));
    this->ui->mf_outputformat_formatoptions_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->mf_outputformat_formatoptions_list->verticalHeader()->hide();
    
    ui->mf_map_outputformat->addItems(MapfileParser::imageTypes);
    ui->mf_outputformat_driver->addItems(MapfileParser::drivers);
    
    this->connect(ui->outputformat_new, SIGNAL(clicked()), SLOT(addNewOutputFormat()));
    this->connect(ui->mf_outputformat_list, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(refreshOutputFormatTab(const QModelIndex &)));
    this->connect(ui->outputformat_edit, SIGNAL(clicked()), SLOT(refreshOutputFormatTab()));
    
    this->connect(ui->mf_outputformat_driver, SIGNAL(currentIndexChanged(const QString &)), SLOT(refreshGdalOgrDriverCombo(const QString &)));
    ui->mf_map_projection->addItem(this->mapfile->getMapProjection());
    
    this->connect(ui->mf_outputformat_options_add, SIGNAL(clicked()), SLOT(addFormatOption()));
    this->connect(ui->mf_outputformat_options_del, SIGNAL(clicked()), SLOT(removeFormatOptions()));


    //Extent
    ui->mf_map_extent_top->setText(QString::number(this->mapfile->getMapExtentMaxY()));
    ui->mf_map_extent_bottom->setText(QString::number(this->mapfile->getMapExtentMinY()));
    ui->mf_map_extent_right->setText(QString::number(this->mapfile->getMapExtentMaxX()));
    ui->mf_map_extent_left->setText(QString::number(this->mapfile->getMapExtentMinX()));

    /** Path tab **/
    ui->mf_map_shapepath->setText(this->mapfile->getShapepath());
    ui->mf_map_symbolset->setText(this->mapfile->getSymbolSet());
    ui->mf_map_fontset->setText(this->mapfile->getFontSet());

    /** Advanced tab **/

    //connect angle value
    this->connect(ui->mf_map_angle_slider, SIGNAL(valueChanged(int)), SLOT(angleSliderChanged(int)));
    this->connect(ui->mf_map_angle, SIGNAL(valueChanged(int)), SLOT(angleSpinChanged(int)));
    //connect projlib browser
    this->connect(ui->mf_map_config_projlib_browse, SIGNAL(clicked()), SLOT(browseProjlibFile()));
    //connect encryption key browser
    this->connect(ui->mf_map_config_encryption_browse, SIGNAL(clicked()), SLOT(browseEncryptionFile()));
    //connect imagecolor
    this->connect(ui->mf_map_imagecolor, SIGNAL(clicked()), SLOT(setImageColor()));

    //fill in form
    ui->mf_map_resolution->setValue(this->mapfile->getResolution());
    ui->mf_map_defresolution->setValue(this->mapfile->getDefResolution());
    ui->mf_map_angle_slider->setValue(this->mapfile->getAngle());

    QList<int> colorList = this->mapfile->getImageColor();
    QColor curColor = QColor(colorList.at(0), colorList.at(1), colorList.at(2));
    ui->mf_map_imagecolor->setPalette(QPalette(curColor));

    ui->mf_map_angle->setValue(this->mapfile->getAngle());
    ui->mf_map_templatepattern->setText(this->mapfile->getTemplatePattern());
    ui->mf_map_datapattern->setText(this->mapfile->getDataPattern());
    ui->mf_map_config_contexturl->setText(this->mapfile->getConfigOption("CGI_CONTEXT_URL"));
    ui->mf_map_config_encryption->setText(this->mapfile->getConfigOption("MS_ENCRYPTION_KEY"));

    if (! this->mapfile->getConfigOption("MS_NONSQUARE").isEmpty()) {
        ui->mf_map_config_squarepixel_on->setChecked(true);
        ui->mf_map_config_squarepixel_off->setChecked(false);
    } else {
        ui->mf_map_config_squarepixel_on->setChecked(false);
        ui->mf_map_config_squarepixel_off->setChecked(true);
    }

    ui->mf_map_config_projlib->setText(this->mapfile->getConfigOption("PROJ_LIB"));

    /** OGC Standard / inspire tab **/

    //connect
    this->connect(ui->mf_ogc_enable, SIGNAL(toggled(bool)), SLOT(enableOgcStandardFrame(bool)));
    this->connect(ui->mf_map_web_md_options_add, SIGNAL(clicked()), SLOT(addOgcMetadata()));
    this->connect(ui->mf_map_web_md_options_del, SIGNAL(clicked()), SLOT(removeOgcMetadatas()));

    //fill in forms
    ui->mf_map_web_md_wms_title->setText(this->mapfile->getMetadataWmsTitle());
    ui->mf_map_web_md_wfs_title->setText(this->mapfile->getMetadataWfsTitle());
    ui->mf_map_web_md_wms_onlineresource->setText(this->mapfile->getMetadataWmsOnlineresource());
    ui->mf_map_web_md_wfs_onlineresource->setText(this->mapfile->getMetadataWfsOnlineresource());
    ui->mf_map_web_md_wms_srs->setText(this->mapfile->getMetadataWmsSrs());
    ui->mf_map_web_md_wfs_srs->setText(this->mapfile->getMetadataWfsSrs());

    ui->mf_map_web_md_option_name->addItems(MapfileParser::ogcMapOptions);

    // Filling the table by known OGC metadata from the mapfile
    KeyValueModel * kvm = new KeyValueModel(this, MapSettings::OgcFilteredOptions);
    kvm->setData(this->mapfile->getMetadatas());
    ui->mf_map_web_md_options_list->setModel(kvm);
    ui->mf_map_web_md_options_list->verticalHeader()->hide();
    ui->mf_map_web_md_options_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    if (kvm->rowCount() > 0) {
      ui->mf_ogc_enable->setCheckState(Qt::Checked);
    }

    this->ui->mf_map_web_md_wfs_enable_gc->setCheckState(mapfile->wfsGetCapabilitiesEnabled()      ? Qt::Checked : Qt::Unchecked);
    this->ui->mf_map_web_md_wfs_enable_gf->setCheckState(mapfile->wfsGetFeatureEnabled()           ? Qt::Checked : Qt::Unchecked);
    this->ui->mf_map_web_md_wfs_enable_dft->setCheckState(mapfile->wfsDescribeFeatureTypeEnabled() ? Qt::Checked : Qt::Unchecked);
    this->ui->mf_map_web_md_wms_enable_gm->setCheckState(mapfile->wmsGetMapEnabled()               ? Qt::Checked : Qt::Unchecked);
    this->ui->mf_map_web_md_wms_enable_glg->setCheckState(mapfile->wmsGetLegendGraphicEnabled()    ? Qt::Checked : Qt::Unchecked);
    this->ui->mf_map_web_md_wms_enable_gc->setCheckState(mapfile->wmsGetCapabilitiesEnabled()      ? Qt::Checked : Qt::Unchecked);
    this->ui->mf_map_web_md_wms_enable_gfi->setCheckState(mapfile->wmsGetFeatureInfoEnabled()      ? Qt::Checked : Qt::Unchecked);

    /** Debug tab **/

    
    this->connect(ui->mf_map_shapepath_browse, SIGNAL(clicked()), SLOT(browseShapepath()));
    this->connect(ui->mf_map_fontset_browse, SIGNAL(clicked()), SLOT(browseFontsetFile()));
    this->connect(ui->mf_map_symbolset_browse, SIGNAL(clicked()), SLOT(browseSymbolsetFile()));
    this->connect(ui->mf_map_debug_on, SIGNAL(toggled(bool)), SLOT(enableDebugBox(bool)));

    if( this->mapfile->getDebug() )
    {
        ui->mf_map_debug_off->setChecked(false);
        ui->mf_map_debug_on->setChecked(true);
        ui->mf_map_debug->setValue(this->mapfile->getDebug());
        ui->mf_map_debug->setEnabled(true);
    } else {
        ui->mf_map_debug_off->setChecked(true);
        ui->mf_map_debug_off->setChecked(false);
        ui->mf_map_debug->setValue(this->mapfile->getDebug());
        ui->mf_map_debug->setEnabled(false);
    }
    //TODO: add option for relative/absolute debug file in forms.
    //TODO: Test if file exist or form is empty, if not warn user
    this->connect(ui->mf_map_config_errorFile_browse, SIGNAL(clicked()), SLOT(browseDebugFile()));
    ui->mf_map_config_errorFile->setText(this->mapfile->getConfigOption("MS_ERRORFILE"));

    ui->mf_map_config_missingdata->addItems(MapfileParser::missingData);
    if (! this->mapfile->getConfigOption("ON_MISSING_DATA").isEmpty()) {
        ui->mf_map_config_missingdata->setCurrentIndex(MapfileParser::missingData.lastIndexOf(this->mapfile->getConfigOption("ON_MISSING_DATA")));
    }


}

void MapSettings::saveMapSettings() {
    // name has changed
    if (ui->mf_map_name->text() != this->mapfile->getMapName())
      ((MainWindow *) parent())->pushUndoStack(new ChangeMapNameCommand(ui->mf_map_name->text(), this->mapfile));

    /** General tab **/

    // status has changed
    if ((ui->mf_map_status_on->isChecked() && ! this->mapfile->getMapStatus()) ||
        (ui->mf_map_status_off->isChecked() && this->mapfile->getMapStatus())) {
      ((MainWindow *) parent())->pushUndoStack(new ChangeMapStatusCommand(ui->mf_map_status_on->isChecked(), this->mapfile));
    }

    // size has changed
    if ((this->mapfile->getMapWidth() != ui->mf_map_size_width->value()) ||
        (this->mapfile->getMapHeight() != ui->mf_map_size_height->value()))
    {
      ((MainWindow *) parent())->pushUndoStack(new SetMapSizeCommand(ui->mf_map_size_width->value(), ui->mf_map_size_height->value(), this->mapfile));
    }

    // Max size has changed
    if (this->mapfile->getMapMaxsize() != ui->mf_map_maxsize->value()) {
      ((MainWindow *) parent())->pushUndoStack(new SetMapMaxSizeCommand(ui->mf_map_maxsize->value(), this->mapfile));
    }

    //units
    int current_unit = MapfileParser::units.indexOf(ui->mf_map_units->currentText());
    if (this->mapfile->getMapUnits() != current_unit) {
      ((MainWindow *) parent())->pushUndoStack(new SetMapUnitsCommand(current_unit, this->mapfile));
    }

    //TODO: default outputformat ?
    //this->mapfile->setOutputformat(ui->mf_map_ouputformat->currentText());

    //projection
    if (this->mapfile->getMapProjection() != ui->mf_map_projection->currentText()) {
      ((MainWindow *) parent())->pushUndoStack(new SetMapProjectionCommand(ui->mf_map_projection->currentText(), this->mapfile));
    }

    //extent

    if ((ui->mf_map_extent_left->text().toFloat()      != this->mapfile->getMapExtentMinX())
        || (ui->mf_map_extent_bottom->text().toFloat() != this->mapfile->getMapExtentMinY())
        || (ui->mf_map_extent_right->text().toFloat()  != this->mapfile->getMapExtentMaxX())
        || (ui->mf_map_extent_top->text().toFloat()    != this->mapfile->getMapExtentMaxY())) {
      ((MainWindow *) parent())->pushUndoStack(new SetMapExtentCommand(ui->mf_map_extent_left->text().toFloat(),
                                                            ui->mf_map_extent_bottom->text().toFloat(),
                                                            ui->mf_map_extent_right->text().toFloat(),
                                                            ui->mf_map_extent_top->text().toFloat(),
                                                            this->mapfile));

    }


    /** Debug tab **/
    if ((this->mapfile->getDebug()  == 0 && ui->mf_map_debug_on->isChecked())
      || (this->mapfile->getDebug() != 0 && ui->mf_map_debug_off->isChecked())) {
      ((MainWindow *) parent())->pushUndoStack(new SetMapDebugCommand(ui->mf_map_debug_on->isChecked() ? ui->mf_map_debug->value() : 0,
                                                           this->mapfile));
    }

    if (ui->mf_map_config_errorFile->text() != this->mapfile->getMetadata("ms_errorfile")) {
      ((MainWindow *) parent())->pushUndoStack(new SetMetadataCommand("ms_errorfile", ui->mf_map_config_errorFile->text(), this->mapfile));
    }
    if (ui->mf_map_config_missingdata->currentText() != this->mapfile->getMetadata("missingdata")) {
      ((MainWindow *) parent())->pushUndoStack(new SetMetadataCommand("missingdata", ui->mf_map_config_missingdata->currentText(), this->mapfile));
    }

    /** Path tab **/
    if (this->mapfile->getShapepath() != ui->mf_map_shapepath->text()) {
      ((MainWindow *) parent())->pushUndoStack(new SetShapePathCommand(ui->mf_map_shapepath->text(), this->mapfile));
    }
    if (this->mapfile->getSymbolSet() != ui->mf_map_symbolset->text()) {
      ((MainWindow *) parent())->pushUndoStack(new SetSymbolSetCommand(ui->mf_map_symbolset->text(), this->mapfile));
    }
    if (this->mapfile->getFontSet() != ui->mf_map_fontset->text()) {
      ((MainWindow *) parent())->pushUndoStack(new SetFontSetCommand(ui->mf_map_fontset->text(), this->mapfile));
    }

    /** Advanced tab **/
    if (this->mapfile->getResolution() != ui->mf_map_resolution->value()) {
      ((MainWindow *) parent())->pushUndoStack(new SetResolutionCommand(ui->mf_map_resolution->value(), this->mapfile));
    }
    if (this->mapfile->getDefResolution() != ui->mf_map_defresolution->value()) {
      ((MainWindow *) parent())->pushUndoStack(new SetDefResolutionCommand(ui->mf_map_defresolution->value(), this->mapfile));
    }
    if (this->mapfile->getAngle() != ui->mf_map_angle->value()) {
      ((MainWindow *) parent())->pushUndoStack(new SetAngleCommand(ui->mf_map_angle->value(), this->mapfile));
    }


    // TODO
    QColor imageColor = ui->mf_map_imagecolor->palette().color(QWidget::backgroundRole());
    this->mapfile->setImageColor(imageColor.red(), imageColor.green(), imageColor.blue());
    
    if (this->mapfile->getTemplatePattern() != ui->mf_map_templatepattern->text()) {
     ((MainWindow *) parent())->pushUndoStack(new SetTemplatePatternCommand(ui->mf_map_templatepattern->text(), this->mapfile));
    }
    
    this->mapfile->setDataPattern(ui->mf_map_datapattern->text());
    this->mapfile->setMetadata("CGI_CONTEXT_URL", ui->mf_map_config_contexturl->text());
    this->mapfile->setMetadata("MS_ENCRYPTION_KEY", ui->mf_map_config_encryption->text());
    if (ui->mf_map_config_squarepixel_on->isChecked()) {
        this->mapfile->setMetadata("MS_NONSQUARE", "ON");
    } else if (ui->mf_map_config_squarepixel_off->isChecked()) {
        this->mapfile->setMetadata("MS_NONSQUARE", "OFF");
    }
    this->mapfile->setMetadata("PROJ_LIB", ui->mf_map_config_projlib->text());

    /** Outputformat tab **/


    /** OGC tab **/

    if (ui->mf_map_web_md_wms_title != ui->mf_map_web_md_wfs_title) {
      this->mapfile->setMetadata("WMS_TITLE", ui->mf_map_web_md_wms_title->text());
      this->mapfile->setMetadata("WFS_TITLE", ui->mf_map_web_md_wfs_title->text());
    } else {
      this->mapfile->setMetadata("OWS_TITLE", ui->mf_map_web_md_wms_title->text());
    }
    //TODO: wms_enabled_request and wfs_enabled_request

    if (ui->mf_map_web_md_wms_onlineresource->text() != ui->mf_map_web_md_wfs_onlineresource->text()) {
        this->mapfile->setMetadata("WMS_ONLINERESOURCE", ui->mf_map_web_md_wms_onlineresource->text());
        this->mapfile->setMetadata("WFS_ONLINERESOURCE", ui->mf_map_web_md_wfs_onlineresource->text());
    } else {
        this->mapfile->setMetadata("OWS_ONLINERESOURCE", ui->mf_map_web_md_wms_onlineresource->text());
    }
    if (ui->mf_map_web_md_wms_srs->text() != ui->mf_map_web_md_wfs_srs->text()) {
        this->mapfile->setMetadata("WMS_SRS", ui->mf_map_web_md_wms_srs->text());
        this->mapfile->setMetadata("WFS_SRS", ui->mf_map_web_md_wfs_srs->text());
    } else { 
        this->mapfile->setMetadata("OWS_SRS", ui->mf_map_web_md_wms_srs->text());
    }
    QStandardItemModel * mod = (QStandardItemModel *) ui->mf_map_web_md_options_list->model();
    if (mod) {
        //TODO: loop on custom metadata list
    }
}


// slots

void MapSettings::addOgcMetadata() {
  QString key   = this->ui->mf_map_web_md_option_name->currentText();
  if (key.isEmpty())
    return;

  QString value = this->ui->mf_map_web_md_option_value->text();
  ((KeyValueModel *) this->ui->mf_map_web_md_options_list->model())->addData(key,value);
  this->ui->mf_map_web_md_options_list->resizeColumnsToContents();
}

void MapSettings::removeOgcMetadatas() {
  QModelIndexList selMds = this->ui->mf_map_web_md_options_list->selectionModel()->selectedRows();
  ((KeyValueModel *) this->ui->mf_map_web_md_options_list->model())->removeDataAt(selMds);
}

void MapSettings::addFormatOption() {
  QString key   = this->ui->mf_outputformat_option_name->text();
  if (key.isEmpty())
    return;

  QString value = this->ui->mf_outputformat_option_value->text();
  ((KeyValueModel *) this->ui->mf_outputformat_formatoptions_list->model())->addData(key,value);
  this->ui->mf_outputformat_formatoptions_list->resizeColumnsToContents();
}

void MapSettings::removeFormatOptions() {
  QModelIndexList selMds = this->ui->mf_outputformat_formatoptions_list->selectionModel()->selectedRows();
  ((KeyValueModel *) this->ui->mf_outputformat_formatoptions_list->model())->removeDataAt(selMds);
}

void MapSettings::setImageColor() {
    QColor curColor = ui->mf_map_imagecolor->palette().color(QWidget::backgroundRole());
    QColor color = QColorDialog::getColor(curColor, this);
    if (color.isValid()) {
        ui->mf_map_imagecolor->setPalette(QPalette(color));
    }
}

void MapSettings::enableOgcStandardFrame(bool checked) {
    ui->mf_ogc_frame->setEnabled(checked);
}

void MapSettings::enableDebugBox(bool checked) {
    ui->mf_map_debug->setEnabled(checked);
}

void MapSettings::angleSliderChanged(int value) {
    ui->mf_map_angle->setValue(value);
}
void MapSettings::angleSpinChanged(int value) {
    ui->mf_map_angle_slider->setValue(value);
}

void MapSettings::addNewOutputFormat() {
  if(ui->outputFormatForm->isEnabled()) {
    if (warnIfActiveSession() == QMessageBox::Yes) {
      this->reinitOutputFormatForm();
    }
    else {
      return;
    }
  }
  this->toggleOutputFormatsWidgets(true);
  
  QList<OutputFormat *> lst = ((OutputFormatsModel *) this->outputFormatsMapper->model())->getEntries();
  QString templ = QString("outfmt%1");
  int idx = 1;
  QStringList ofNames = QStringList();
  for (int i = 0; i < lst.size(); ++i) {
    ofNames.append(lst.at(i)->getName());
  }
  QString curGenOf = templ.arg(idx);
  while (ofNames.contains(curGenOf)) 
    curGenOf = templ.arg(++idx);

  OutputFormat * of = new OutputFormat(curGenOf);
  of->setState(OutputFormat::ADDED);

  lst.append(of);
  ((OutputFormatsModel *) this->outputFormatsMapper->model())->setEntries(lst);

}

void MapSettings::reinitOutputFormatForm() {
  ui->mf_outputformat_name->clear();
  ui->mf_outputformat_driver->clear();
  ui->mf_outputformat_transparent_on->setEnabled(true);
  ui->mf_outputformat_transparent_on->setChecked(true);
  ui->mf_outputformat_transparent_off->setEnabled(true);
  ui->mf_outputformat_transparent_off->setChecked(false);
  ui->mf_outputformat_extension->clear();
  ui->mf_outputformat_mimetype->clear();
  ui->mf_outputformat_option_name->clear();
  ui->mf_outputformat_option_value->clear();
  ui->mf_outputformat_formatoptions_list->clearSpans();
  return;
}

QMessageBox::StandardButton MapSettings::warnIfActiveSession() {
  return QMessageBox::question(this, tr("Warning: currently editing"),
			       tr("You are currently editing an output format. "),
			       //"Opening another one will discard your "
			       //"current changes. Are you sure ?"),
			       QMessageBox::Yes | QMessageBox::No);
}

/** Following method should be refactored **/
void MapSettings::browseProjlibFile() {
      //TODO: should be defaulted to dirname of mapfile if option relativepath is set to on
      QString prevDirPath = QDir::homePath();

      QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory Files"), prevDirPath);
      // open file dialog has been discarded (escape)
      if (dirName.isEmpty()) {
            return;
      }
      //TODO: should we used relatif or absolute path?
      ui->mf_map_config_projlib->setText(dirName);
}

void MapSettings::browseEncryptionFile() {

      //TODO: should be defaulted to dirname of mapfile if option relativepath is set to on
      QString prevFilePath = QDir::homePath();

      QString fileName = QFileDialog::getOpenFileName(this, tr("Open Encryption File"), prevFilePath, tr("Encryption file (*)"));
      // open file dialog has been discarded (escape)
      if (fileName.isEmpty()) {
            return;
      }
      //TODO: should we used relatif or absolute path? if dirname = dirname of mapfile then basename(fileName)
      ui->mf_map_config_encryption->setText(fileName);
}


void MapSettings::browseDebugFile() {

      //TODO: should be defaulted to dirname of mapfile if option relativepath is set to on
      QString prevFilePath = QDir::homePath();

      QString fileName = QFileDialog::getOpenFileName(this, tr("Open debug File"), prevFilePath, tr("Debug file (*.log)"));
      // open file dialog has been discarded (escape)
      if (fileName.isEmpty()) {
            return;
      }
      //TODO: should we used relatif or absolute path? if dirname = dirname of mapfile then basename(fileName)
      ui->mf_map_config_errorFile->setText(fileName);
}

void MapSettings::browseSymbolsetFile() {

      //TODO: should be defaulted to dirname of mapfile if option relativepath is set to on
      QString prevFilePath = QDir::homePath();

      QString fileName = QFileDialog::getOpenFileName(this, tr("Open symbolset File"), prevFilePath, tr("Symbolset file (*.sym)"));
      // open file dialog has been discarded (escape)
      if (fileName.isEmpty()) {
            return;
      }
      //TODO: should we used relatif or absolute path? if dirname = dirname of mapfile then basename(fileName)
      ui->mf_map_symbolset->setText(fileName);
}

void MapSettings::browseFontsetFile() {

      //TODO: should be defaulted to dirname of mapfile if option relativepath is set to on
      QString prevFilePath = QDir::homePath();

      QString fileName = QFileDialog::getOpenFileName(this, tr("Open fontset File"), prevFilePath, tr("Fontset file (*.font)"));
      // open file dialog has been discarded (escape)
      if (fileName.isEmpty()) {
            return;
      }
      //TODO: should we used relatif or absolute path? if dirname = dirname of mapfile then basename(fileName)
      ui->mf_map_fontset->setText(fileName);
}

void MapSettings::browseShapepath() {
      //TODO: should be defaulted to dirname of mapfile if option relativepath is set to on
      QString prevDirPath = QDir::homePath();

      QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory Files"), prevDirPath);
      // open file dialog has been discarded (escape)
      if (dirName.isEmpty()) {
            return;
      }
      //TODO: should we used relatif or absolute path?
      ui->mf_map_shapepath->setText(dirName);
}

void MapSettings::accept() {
    this->saveMapSettings();

    QDialog::accept();
}
void MapSettings::refreshGdalOgrDriverCombo(const QString &s) {
    if ((s == "GDAL") || (s == "OGR")) {
      this->ui->gdaldriver_label->setEnabled(true);
      this->ui->mf_gdal_ogr_driver->setEnabled(true);
      if (s == "GDAL") {
        this->ui->mf_gdal_ogr_driver->clear();
        this->ui->mf_gdal_ogr_driver->addItems(this->mapfile->getGdalGdalDrivers());
      } else {
        this->ui->mf_gdal_ogr_driver->clear();
        this->ui->mf_gdal_ogr_driver->addItems(this->mapfile->getGdalOgrDrivers());
      }
    } else {
      this->ui->gdaldriver_label->setDisabled(true);
      this->ui->mf_gdal_ogr_driver->setDisabled(true);
      this->ui->mf_gdal_ogr_driver->setCurrentIndex(0);
    }
}

void MapSettings::refreshOutputFormatTab(void) {
 this->refreshOutputFormatTab(this->ui->mf_outputformat_list->currentIndex());
}

void MapSettings::refreshOutputFormatTab(const QModelIndex &i) {
  if(ui->outputFormatForm->isEnabled()) {
    if (warnIfActiveSession() == QMessageBox::Yes) {
      this->reinitOutputFormatForm();
    }
    else {
      return;
    }
  }
  this->outputFormatsMapper->setCurrentModelIndex(i);
  OutputFormat * fmt = ((OutputFormatsModel *) this->outputFormatsMapper->model())->getOutputFormat(i);
  if (! fmt)
    return;

  // Updating the format options list
  KeyValueModel * mdl = (KeyValueModel *) this->ui->mf_outputformat_formatoptions_list->model();
  mdl->setData(fmt->getFormatOptions());
  ui->mf_outputformat_formatoptions_list->resizeColumnsToContents();
  this->toggleOutputFormatsWidgets(true);
}

void MapSettings::toggleOutputFormatsWidgets(const bool &enable) {
  this->ui->outputFormatForm->setEnabled(enable);
  this->ui->mf_outputformat_form_buttons->setEnabled(enable);
}


/** End SLOTS **/

MapSettings::~MapSettings() {
  // mapfile lifecycle should be managed elsewhere
  delete ui;
}

QStringList MapSettings::OgcFilteredOptions = QStringList() << "wms_title" << "wfs_title" << "ows_title"
  << "wms_enable_request" <<  "wfs_enable_request" << "ows_enable_request"
  << "wms_onlineresource" << "wfs_onlineresource" << "ows_onlineresource"
  << "wms_srs" << "wfs_srs" << "ows_srs";


