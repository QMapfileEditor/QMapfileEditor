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

#include "mainwindow.h"

#include "mapsettings.h"
#include "ui_mapsettings.h"

#include <QDebug>

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
    // default output format
    this->populateDefaultOutputFormatList();
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

    //Extent
    ui->mf_map_extent_top->setText(QString::number(this->mapfile->getMapExtentMaxY()));
    ui->mf_map_extent_bottom->setText(QString::number(this->mapfile->getMapExtentMinY()));
    ui->mf_map_extent_right->setText(QString::number(this->mapfile->getMapExtentMaxX()));
    ui->mf_map_extent_left->setText(QString::number(this->mapfile->getMapExtentMinX()));

    //Projection
    if((this->mapfile->getMapProjection().startsWith(QString("epsg"))) || (this->mapfile->getMapProjection().startsWith(QString("+init=")))) {
        ui->mf_map_projection_btproj->setChecked(false);
        ui->mf_map_projection_btepsg->setChecked(true);
        ui->mf_map_projection_btlink->setEnabled(true);
        //add a slot to open an url to http://epsg.io website
        this->connect(ui->mf_map_projection_btlink, SIGNAL(clicked()), SLOT(openProjectionInfo()));
    } else {
        ui->mf_map_projection_btproj->setChecked(true);
        ui->mf_map_projection_btepsg->setChecked(false);
        ui->mf_map_projection_info->setEnabled(false);
    }
    //Slot to manage change of projection mode (proj/wkt vs epsg)
    this->connect(ui->mf_map_projection_btproj, SIGNAL(clicked()), SLOT(switchProjectionMode()));
    this->connect(ui->mf_map_projection_btepsg, SIGNAL(clicked()), SLOT(switchProjectionMode()));
    ui->mf_map_projection->addItem(this->mapfile->getMapProjection());

    /** Path tab **/
    //connect relatif path for shapepath, symbolset and fontset
    this->connect(ui->mf_map_shapepath_relative, SIGNAL(clicked()), SLOT(enableRelativePathShapepath()));
    this->connect(ui->mf_map_symbolset_relative, SIGNAL(clicked()), SLOT(enableRelativePathSymbolset()));
    //TO REMOVE : this->connect(ui->mf_map_fontset_relative, SIGNAL(clicked()), SLOT(enableRelativePathFontset()));

    ui->mf_map_shapepath->setText(this->mapfile->getShapepath());

    ui->mf_map_shapepath_relative->setChecked(true);
    if((QFileInfo (this->mapfile->getShapepath())).isAbsolute()) {
      ui->mf_map_shapepath_relative->setChecked(false);
    }

    ui->mf_map_symbolset->setText(this->mapfile->getSymbolSet());
    ui->mf_map_symbolset_relative->setChecked(true);
    if((QFileInfo (this->mapfile->getSymbolSet())).isAbsolute()) {
      ui->mf_map_symbolset_relative->setChecked(false);
    }

    //TO REMOVE ui->mf_map_fontset->setText(this->mapfile->getFontSet());
    //TO REMOVE ui->mf_map_fontset_relative->setChecked(true);
    //TO REMOVE if((QFileInfo (this->mapfile->getFontSet())).isAbsolute()) {
    //  ui->mf_map_fontset_relative->setChecked(false);
    //}

    /** Advanced tab **/

    //connect angle value
    this->connect(ui->mf_map_angle_slider, SIGNAL(valueChanged(int)), SLOT(angleSliderChanged(int)));
    this->connect(ui->mf_map_angle, SIGNAL(valueChanged(int)), SLOT(angleSpinChanged(int)));
    //connect projlib browser
    this->connect(ui->mf_map_config_projlib_browse, SIGNAL(clicked()), SLOT(browseProjlibFile()));
    //connect encryption key browser
    this->connect(ui->mf_map_config_encryption_browse, SIGNAL(clicked()), SLOT(browseEncryptionFile()));
    // imagecolor enable logic
    this->connect(ui->mf_map_imagecolor_enable, SIGNAL(stateChanged(int)), SLOT(toggleImageColor(int)));
    //connect imagecolor
    this->connect(ui->mf_map_imagecolor, SIGNAL(clicked()), SLOT(setImageColor()));
    //connect relatif path for proj lib and encryption
    this->connect(ui->mf_map_config_projlib_relative, SIGNAL(clicked()), SLOT(enableRelativePathProjlib()));
    this->connect(ui->mf_map_config_encryption_relative, SIGNAL(clicked()), SLOT(enableRelativePathEncryption()));

    //fill in form
    ui->mf_map_resolution->setValue(this->mapfile->getResolution());
    ui->mf_map_defresolution->setValue(this->mapfile->getDefResolution());
    ui->mf_map_angle_slider->setValue(this->mapfile->getAngle());

    QColor curColor = this->mapfile->getImageColor();
    if (curColor.isValid()) {
      ui->mf_map_imagecolor_enable->setCheckState(Qt::Checked);
      ui->mf_map_imagecolor->setPalette(QPalette(curColor));
    } else {
      ui->mf_map_imagecolor_enable->setCheckState(Qt::Unchecked);
    }
    ui->mf_map_angle->setValue(this->mapfile->getAngle());
    ui->mf_map_templatepattern->setText(this->mapfile->getTemplatePattern());
    ui->mf_map_datapattern->setText(this->mapfile->getDataPattern());

    ui->mf_map_config_contexturl->setText(this->mapfile->getConfigOption("CGI_CONTEXT_URL"));

    ui->mf_map_config_encryption->setText(this->mapfile->getConfigOption("MS_ENCRYPTION_KEY"));
    ui->mf_map_config_encryption_relative->setChecked(true);
    if((QFileInfo (this->mapfile->getConfigOption("MS_ENCRYPTION_KEY"))).isAbsolute()) {
      ui->mf_map_config_encryption_relative->setChecked(false);
    }

    QString msNonSquare = this->mapfile->getConfigOption("MS_NONSQUARE").toLower();
      if (msNonSquare == "yes") {
        ui->mf_map_config_squarepixel_on->setChecked(true);
        ui->mf_map_config_squarepixel_off->setChecked(false);
    } else {
        ui->mf_map_config_squarepixel_on->setChecked(false);
        ui->mf_map_config_squarepixel_off->setChecked(true);
    }

    ui->mf_map_config_projlib->setText(this->mapfile->getConfigOption("PROJ_LIB"));
    ui->mf_map_config_projlib_relative->setChecked(true);
    if((QFileInfo (this->mapfile->getConfigOption("PROJ_LIB"))).isAbsolute()) {
      ui->mf_map_config_projlib_relative->setChecked(false);
    }

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
    this->outputFormatsMapper->addMapping(ui->mf_gdal_ogr_driver,        OutputFormatsModel::GdalDriver);
    this->outputFormatsMapper->addMapping(ui->mf_outputformat_extension, OutputFormatsModel::Extension);
    this->outputFormatsMapper->addMapping(ui->mf_outputformat_imagemode, OutputFormatsModel::ImageMode);
    this->outputFormatsMapper->addMapping(ui->mf_outputformat_mimetype,  OutputFormatsModel::MimeType);
    this->ui->mf_outputformat_formatoptions_list->setModel(new KeyValueModel(this));
    this->ui->mf_outputformat_formatoptions_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->mf_outputformat_formatoptions_list->verticalHeader()->hide();

    ui->mf_outputformat_driver->addItems(MapfileParser::drivers);

    this->connect(ui->outputformat_new, SIGNAL(clicked()), SLOT(addNewOutputFormat()));
    this->connect(ui->mf_outputformat_list, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(refreshOutputFormatTab(const QModelIndex &)));
    this->connect(ui->outputformat_edit, SIGNAL(clicked()), SLOT(refreshOutputFormatTab()));
    this->connect(ui->outputformat_delete, SIGNAL(clicked()), SLOT(removeOutputFormat()));
    this->connect(ui->mf_outputformat_driver, SIGNAL(currentIndexChanged(const QString &)), SLOT(refreshGdalOgrDriverCombo(const QString &)));

    this->connect(ui->mf_outputformat_options_add, SIGNAL(clicked()), SLOT(addFormatOption()));
    this->connect(ui->mf_outputformat_options_del, SIGNAL(clicked()), SLOT(removeFormatOptions()));
    // accept / reject outputformat modifications
    this->connect(ui->mf_outputformat_form_buttons, SIGNAL(clicked(QAbstractButton *)), SLOT(handleOutputFormatFormClick(QAbstractButton *)));


    /** Web Services / inspire tab **/

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
    //TO REMOVE this->connect(ui->mf_map_fontset_browse, SIGNAL(clicked()), SLOT(browseFontsetFile()));
    this->connect(ui->mf_map_symbolset_browse, SIGNAL(clicked()), SLOT(browseSymbolsetFile()));
    this->connect(ui->mf_map_debug_on, SIGNAL(toggled(bool)), SLOT(enableDebugBox(bool)));
    this->connect(ui->mf_map_config_errorFile_relative, SIGNAL(clicked()), SLOT(enableRelativePathDebug()));
    //TODO: Test if file exist or form is empty, if not warn user
    this->connect(ui->mf_map_config_errorFile_browse, SIGNAL(clicked()), SLOT(browseDebugFile()));

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


    ui->mf_map_config_errorFile->setText(this->mapfile->getConfigOption("MS_ERRORFILE"));
    ui->mf_map_config_errorFile_relative->setChecked(true);
    if((QFileInfo (this->mapfile->getConfigOption("MS_ERRORFILE"))).isAbsolute()) {
      ui->mf_map_config_errorFile_relative->setChecked(false);
    }

    ui->mf_map_config_missingdata->addItems(MapfileParser::missingData);
    if (! this->mapfile->getConfigOption("ON_MISSING_DATA").isEmpty()) {
        ui->mf_map_config_missingdata->setCurrentIndex(MapfileParser::missingData.lastIndexOf(this->mapfile->getConfigOption("ON_MISSING_DATA")));
    }


}


void MapSettings::populateDefaultOutputFormatList(void) {
  ui->mf_map_outputformat->clear();
  QList<OutputFormat *> fmts = this->mapfile->getOutputFormats();
  ui->mf_map_outputformat->addItems(MapfileParser::imageTypes);

  for (int i =0; i < fmts.size(); i++)
    ui->mf_map_outputformat->addItem(fmts[i]->getName());

  ui->mf_map_outputformat->setCurrentIndex(ui->mf_map_outputformat->findText(this->mapfile->getDefaultOutputFormat()));

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

    // default outputformat (either mimetype or format name)
    if (this->mapfile->getDefaultOutputFormat() != ui->mf_map_outputformat->currentText()) {
      ((MainWindow *) parent())->pushUndoStack(new SetDefaultOutputFormatCommand(ui->mf_map_outputformat->currentText(), this->mapfile));
      //this->mapfile->setDefaultOutputFormat(ui->mf_map_outputformat->currentText());
    }

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

    // error file (map options)
    if (ui->mf_map_config_errorFile->text() != this->mapfile->getConfigOption("MS_ERRORFILE")) {
      ((MainWindow *) parent())->pushUndoStack(new SetConfigOptionCommand("MS_ERRORFILE", ui->mf_map_config_errorFile->text(), this->mapfile));
    }
    // missing data (map options)
    if (ui->mf_map_config_missingdata->currentText() != this->mapfile->getConfigOption("ON_MISSING_DATA")) {
      ((MainWindow *) parent())->pushUndoStack(new SetConfigOptionCommand("ON_MISSING_DATA", ui->mf_map_config_missingdata->currentText(), this->mapfile));
    }

    /** Path tab **/
    if (this->mapfile->getShapepath() != ui->mf_map_shapepath->text()) {
      ((MainWindow *) parent())->pushUndoStack(new SetShapePathCommand(ui->mf_map_shapepath->text(), this->mapfile));
    }
    if (this->mapfile->getSymbolSet() != ui->mf_map_symbolset->text()) {
      ((MainWindow *) parent())->pushUndoStack(new SetSymbolSetCommand(ui->mf_map_symbolset->text(), this->mapfile));
    }
    //TO REMOVE if (this->mapfile->getFontSet() != ui->mf_map_fontset->text()) {
    //  ((MainWindow *) parent())->pushUndoStack(new SetFontSetCommand(ui->mf_map_fontset->text(), this->mapfile));
    //}

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

    QColor imageColor = ui->mf_map_imagecolor->palette().color(QWidget::backgroundRole());
    if (this->mapfile->getImageColor() != imageColor) {
      ((MainWindow *) parent())->pushUndoStack(new SetImageColorCommand(imageColor, this->mapfile));
    }

    if (this->mapfile->getTemplatePattern() != ui->mf_map_templatepattern->text()) {
     ((MainWindow *) parent())->pushUndoStack(new SetTemplatePatternCommand(ui->mf_map_templatepattern->text(), this->mapfile));
    }

    if (ui->mf_map_datapattern->text() != this->mapfile->getDataPattern()) {
      ((MainWindow *) parent())->pushUndoStack(new SetDataPatternCommand(ui->mf_map_datapattern->text(), this->mapfile));
    }

    // CGI_CONTEXT_URL (map options)
    if (ui->mf_map_config_contexturl->text() != this->mapfile->getConfigOption("CGI_CONTEXT_URL")) {
      ((MainWindow *) parent())->pushUndoStack(new SetConfigOptionCommand("CGI_CONTEXT_URL",
                                                                          ui->mf_map_config_contexturl->text(), this->mapfile));
    }

    // encryption key (map options)
    if (ui->mf_map_config_encryption->text() != this->mapfile->getConfigOption("MS_ENCRYPTION_KEY")) {
      ((MainWindow *) parent())->pushUndoStack(new SetConfigOptionCommand("MS_ENCRYPTION_KEY",
                                                                          ui->mf_map_config_encryption->text(), this->mapfile));
    }

    // MS_NONSQUARE (map options)
    QString msNonSquare = this->mapfile->getConfigOption("MS_NONSQUARE").toLower();
    if (ui->mf_map_config_squarepixel_on->isChecked()) {
      if (msNonSquare != "yes") {
        ((MainWindow *) parent())->pushUndoStack(new SetConfigOptionCommand("MS_NONSQUARE", "yes", this->mapfile));
      }
    } else if (ui->mf_map_config_squarepixel_off->isChecked()) {
       if (msNonSquare != "no") {
        ((MainWindow *) parent())->pushUndoStack(new SetConfigOptionCommand("MS_NONSQUARE", "no", this->mapfile));
      }
    }

    // PROJ_LIB (map option)
    if (ui->mf_map_config_projlib->text() != this->mapfile->getConfigOption("PROJ_LIB")) {
      ((MainWindow *) parent())->pushUndoStack(new SetConfigOptionCommand("PROJ_LIB",
                                                                          ui->mf_map_config_projlib->text(), this->mapfile));
    }

    /** Outputformat tab **/

    // TODO: it might be interesting to save the model as member variable
    // of the class instead of having to cast it all along the code.
    OutputFormatsModel * fmtMdl = (OutputFormatsModel *) (ui->mf_outputformat_list->model());

    QList<OutputFormat *> removedEntries = fmtMdl->getRemovedEntries();

    for (int i = 0 ; i < removedEntries.size(); ++i) {
      ((MainWindow *) parent())->pushUndoStack(new RemoveOutputFormatCommand(removedEntries[i], this->mapfile));
    }

    QList<OutputFormat *> entries = fmtMdl->getEntries();

    for (int i = 0; i < entries.size(); ++i) {
     if (entries[i]->getState() == OutputFormat::UNCHANGED)
       continue;
     if ((entries[i]->getState() == OutputFormat::ADDED) || (entries[i]->getState() == OutputFormat::ADDED_SAVED)) {
       // before inserting, consider the OF as UNCHANGED
       entries[i]->setState(OutputFormat::UNCHANGED);
       ((MainWindow *) parent())->pushUndoStack(new AddNewOutputFormatCommand(entries[i], this->mapfile));
     }
     else if (entries[i]->getState() == OutputFormat::MODIFIED) {
       entries[i]->setState(OutputFormat::UNCHANGED);
       ((MainWindow *) parent())->pushUndoStack(new UpdateOutputFormatCommand(entries[i], this->mapfile));
     }
    }

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

void MapSettings::toggleImageColor(int state) {
  switch(state) {
    case Qt::Unchecked:
      ui->mf_map_imagecolor->setDisabled(true);
      ui->mf_map_imagecolor->setPalette(QPalette(QColor(0xff, 0xff, 0xff)));
      break;
    case Qt::Checked:
      ui->mf_map_imagecolor->setDisabled(false);
      ui->mf_map_imagecolor->setPalette(QPalette(QColor(mapfile->getImageColor())));
      break;
  }
}

void MapSettings::openProjectionInfo() {
    QString epsgCode = ui->mf_map_projection->currentText().replace(QString("+init=epsg:"), QString(""));
    QDesktopServices::openUrl(QUrl("http://epsg.io/"+epsgCode, QUrl::StrictMode));
}

void MapSettings::switchProjectionMode() {
    //if proj4 is checked then ...
    if(ui->mf_map_projection_btproj->isChecked()) {
        //look for wkt string from epsg code
        ui->mf_map_projection_btlink->setEnabled(false);
    } else if (ui->mf_map_projection_btepsg->isChecked()) {
        //look for epsg from wkt string
        ui->mf_map_projection_btlink->setEnabled(true);
    }
}

void MapSettings::handleOutputFormatFormClick(QAbstractButton *b) {

  OutputFormatsModel * mdl =  (OutputFormatsModel *) this->outputFormatsMapper->model();
  QModelIndex curIdx = ui->mf_outputformat_list->currentIndex();
  OutputFormat * fmt = mdl->getOutputFormat(curIdx);

  // save
  if (ui->mf_outputformat_form_buttons->buttonRole(b) == QDialogButtonBox::AcceptRole) {
    // check if there is a name clashing (only if the name has been actually
    // edited)
    if ((ui->mf_outputformat_name->text() != fmt->getName()) && 
      (mdl->nameAlreadyIn(ui->mf_outputformat_name->text()))) {
      QMessageBox::warning(this, tr("Error"), tr("An existing output format is already named  %1. Please select "
                                                 "another name.").arg(ui->mf_outputformat_name->text()));
       return;
    }
    fmt->setName(ui->mf_outputformat_name->text());
    fmt->setExtension(ui->mf_outputformat_extension->text());
    fmt->setImageMode(ui->mf_outputformat_imagemode->currentIndex());
    fmt->setMimeType(ui->mf_outputformat_mimetype->text());
    QString currentDriver = ui->mf_outputformat_driver->currentText();
    if (currentDriver == "GDAL" || currentDriver == "OGR") {
      fmt->setDriver(currentDriver + "/" + ui->mf_gdal_ogr_driver->currentText());
    } else {
      fmt->setDriver(ui->mf_outputformat_driver->currentText());
    }
    fmt->setTransparent(ui->mf_outputformat_transparent_on->isChecked());
    KeyValueModel * fmtOptsMdl = ((KeyValueModel *) this->ui->mf_outputformat_formatoptions_list->model());
    fmt->setFormatOptions(fmtOptsMdl->getData());
    if (fmt->getState() == OutputFormat::ADDED) {
      fmt->setState(OutputFormat::ADDED_SAVED);
    }
    else if (fmt->getState() == OutputFormat::UNCHANGED) {
      fmt->setState(OutputFormat::MODIFIED);
    }
  }
  // Discard
  else if (ui->mf_outputformat_form_buttons->buttonRole(b) == QDialogButtonBox::DestructiveRole) {
    if (fmt->getState() == OutputFormat::ADDED)
      mdl->removeOutputFormat(curIdx);
  }
  this->ui->mf_outputformat_list->clearSelection();
  this->reinitOutputFormatForm();
  this->toggleOutputFormatsWidgets(false);

  this->populateDefaultOutputFormatList();
}

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
    } else {
      return;
    }
  }
  this->toggleOutputFormatsWidgets(true);
  OutputFormatsModel * mdl =  (OutputFormatsModel *) this->outputFormatsMapper->model();
  QList<OutputFormat *> lst = mdl->getEntries();
  QString templ = QString("newOutFmt%1");
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
  mdl->setEntries(lst);

  // Selects the outputformat in the list
  QModelIndex ofIdx = mdl->index(mdl->rowCount() -1);
  ui->mf_outputformat_list->setCurrentIndex(ofIdx);
  this->outputFormatsMapper->setCurrentModelIndex(ofIdx);

}

void MapSettings::removeOutputFormat() {
 QModelIndex idx = this->ui->mf_outputformat_list->currentIndex();
 if (! idx.isValid())
   return;

 OutputFormatsModel * ofMdl = (OutputFormatsModel *) this->outputFormatsMapper->model();
 OutputFormat *of = ofMdl->getOutputFormat(idx);

 // removes the OF from the model
 ofMdl->removeOutputFormat(idx);

 // Removes the outputformat from the default outputformat combo
 // (on main tab)
 if (of) {
   int comboIdx = ui->mf_map_outputformat->findText(of->getOriginalName());
   if (comboIdx != -1)
     ui->mf_map_outputformat->removeItem(comboIdx);
 }
}

QMessageBox::StandardButton MapSettings::warnIfActiveSession() {
  return QMessageBox::question(this, tr("Warning: currently editing"),
			       tr("You are currently editing an output format. "
			       "Opening another one will discard your "
			       "current changes. Are you sure ?"),
			       QMessageBox::Yes | QMessageBox::No);
}

/** Following method should be refactored **/
void MapSettings::browseProjlibFile() {

  QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory Files"), ((MainWindow *) parent())->prevDirPath);
      // open file dialog has been discarded (escape)
      if (dirName.isEmpty()) {
            return;
      }

      if(ui->mf_map_config_projlib_relative->isChecked()) {
	dirName = ((MainWindow *) parent())->mapfiledir.relativeFilePath(dirName);
      }
      ui->mf_map_config_projlib->setText(dirName);
}

void MapSettings::enableRelativePathProjlib() {
  if(ui->mf_map_config_projlib_relative->isChecked()) {
    ui->mf_map_config_projlib->setText(((MainWindow *) parent())->mapfiledir.relativeFilePath(ui->mf_map_config_projlib->text()));
  } else {
    ui->mf_map_config_projlib->setText(QDir (((MainWindow *) parent())->mapfiledir.path() + "/" + ui->mf_map_config_projlib->text()).absolutePath());
  }
}

void MapSettings::browseEncryptionFile() {

      QString fileName = QFileDialog::getOpenFileName(this, tr("Open Encryption File"), ((MainWindow *) parent())->prevDirPath, tr("Encryption file (*)"));
      // open file dialog has been discarded (escape)
      if (fileName.isEmpty()) {
            return;
      }

      if(ui->mf_map_config_encryption_relative->isChecked()) {
	fileName = ((MainWindow *) parent())->mapfiledir.relativeFilePath(fileName);
      }
      ui->mf_map_config_encryption->setText(fileName);
}

void MapSettings::enableRelativePathEncryption() {
  if(ui->mf_map_config_encryption_relative->isChecked()) {
    ui->mf_map_config_encryption->setText(((MainWindow *) parent())->mapfiledir.relativeFilePath(ui->mf_map_config_encryption->text()));
  } else {
    ui->mf_map_config_encryption->setText(((MainWindow *) parent())->mapfiledir.path() + "/" + ui->mf_map_config_encryption->text());
  }
}

void MapSettings::browseDebugFile() {

      QString fileName = QFileDialog::getOpenFileName(this, tr("Open debug File"), ((MainWindow *) parent())->prevDirPath, tr("Debug file (*.log)"));
      // open file dialog has been discarded (escape)
      if (fileName.isEmpty()) {
            return;
      }
      
      if(ui->mf_map_config_errorFile_relative->isChecked()) {
	fileName = ((MainWindow *) parent())->mapfiledir.relativeFilePath(fileName);
      }
      ui->mf_map_config_errorFile->setText(fileName);
}

void MapSettings::enableRelativePathDebug() {
  if(ui->mf_map_config_errorFile_relative->isChecked()) {
    ui->mf_map_config_errorFile->setText(((MainWindow *) parent())->mapfiledir.relativeFilePath(ui->mf_map_config_errorFile->text()));
  } else {
    ui->mf_map_config_errorFile->setText(((MainWindow *) parent())->mapfiledir.path() + "/" + ui->mf_map_config_errorFile->text());
  }
}

void MapSettings::browseSymbolsetFile() {

      QString fileName = QFileDialog::getOpenFileName(this, tr("Open symbolset File"), ((MainWindow *) parent())->prevDirPath, tr("Symbolset file (*.sym)"));
      // open file dialog has been discarded (escape)
      if (fileName.isEmpty()) {
            return;
      }

      if(ui->mf_map_symbolset_relative->isChecked()) {
	fileName = ((MainWindow *) parent())->mapfiledir.relativeFilePath(fileName);
      }
      ui->mf_map_symbolset->setText(fileName);
}

void MapSettings::enableRelativePathSymbolset() {
  if(ui->mf_map_symbolset_relative->isChecked()) {
    ui->mf_map_symbolset->setText(((MainWindow *) parent())->mapfiledir.relativeFilePath(ui->mf_map_symbolset->text()));
  } else {
    ui->mf_map_symbolset->setText(((MainWindow *) parent())->mapfiledir.path() + "/" + ui->mf_map_symbolset->text());
  }
}

//TO REMOVE
/*void MapSettings::browseFontsetFile() {

      QString fileName = QFileDialog::getOpenFileName(this, tr("Open fontset File"), ((MainWindow *) parent())->prevDirPath, tr("Fontset file (*.font)"));
      // open file dialog has been discarded (escape)
      if (fileName.isEmpty()) {
            return;
      }
      
      if(ui->mf_map_fontset_relative->isChecked()) {
	fileName = ((MainWindow *) parent())->mapfiledir.relativeFilePath(fileName);
      }
      ui->mf_map_fontset->setText(fileName);
}

void MapSettings::enableRelativePathFontset() {
  if(ui->mf_map_fontset_relative->isChecked()) {
    ui->mf_map_fontset->setText(((MainWindow *) parent())->mapfiledir.relativeFilePath(ui->mf_map_fontset->text()));
  } else {
    ui->mf_map_fontset->setText(((MainWindow *) parent())->mapfiledir.path() + "/" + ui->mf_map_fontset->text());
  }
}
*/

void MapSettings::browseShapepath() {

  QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory Files"), ((MainWindow *) parent())->prevDirPath);
      // open file dialog has been discarded (escape)
      if (dirName.isEmpty()) {
            return;
      }
      
      if(ui->mf_map_shapepath_relative->isChecked()) {
	dirName = ((MainWindow *) parent())->mapfiledir.relativeFilePath(dirName);
      }
      ui->mf_map_shapepath->setText(dirName);
}

void MapSettings::enableRelativePathShapepath() {
  if(ui->mf_map_shapepath_relative->isChecked()) {
    ui->mf_map_shapepath->setText(((MainWindow *) parent())->mapfiledir.relativeFilePath(ui->mf_map_shapepath->text()));
  } else {
    ui->mf_map_shapepath->setText(((MainWindow *) parent())->mapfiledir.path() + "/" + ui->mf_map_shapepath->text());
  }
}

void MapSettings::accept() {
    this->saveMapSettings();
    // Refreshes the map view
    ((MainWindow *) parent())->updateMapPreview();
    QDialog::accept();
}
void MapSettings::refreshGdalOgrDriverCombo(const QString &s) {
    if ((s == "GDAL") || (s == "OGR")) {
      this->ui->gdaldriver_label->setEnabled(true);
      this->ui->mf_gdal_ogr_driver->setEnabled(true);
      if (s == "GDAL") {
        this->ui->mf_gdal_ogr_driver->clear();
        this->ui->mf_gdal_ogr_driver->addItems(((MainWindow *) parent())->gdalGdalDrivers);
      } else {
        this->ui->mf_gdal_ogr_driver->clear();
        this->ui->mf_gdal_ogr_driver->addItems(((MainWindow *) parent())->gdalOgrDrivers);
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

void MapSettings::reinitOutputFormatForm() {
  ui->mf_outputformat_transparent_on->setEnabled(true);
  ui->mf_outputformat_transparent_on->setChecked(true);
  ui->mf_outputformat_transparent_off->setEnabled(true);
  ui->mf_outputformat_transparent_off->setChecked(false);
  ui->mf_outputformat_formatoptions_list->clearSpans();
  ui->mf_outputformat_name->clear();
  ui->mf_outputformat_driver->setCurrentIndex(0);
  ui->mf_gdal_ogr_driver->setCurrentIndex(0);
  ui->mf_outputformat_extension->clear();
  ui->mf_outputformat_imagemode->setCurrentIndex(0);
  ui->mf_outputformat_mimetype->clear();
  ui->mf_outputformat_option_name->clear();
  ui->mf_outputformat_option_value->clear();
  KeyValueModel * mdl = (KeyValueModel *) this->ui->mf_outputformat_formatoptions_list->model();
  mdl->setData(QHash<QString, QString>());
}

void MapSettings::toggleOutputFormatsWidgets(const bool &enable) {
  // toggles the button to add / edit / delete / clear / import
  // if editing / not editing
  this->ui->outputformat_buttons->setEnabled(! enable);
  this->ui->mf_outputformat_list->setEnabled(! enable);

  this->ui->outputFormatForm->setEnabled(enable);
  this->ui->mf_outputformat_form_buttons->setEnabled(enable);
}

/** End SLOTS **/

MapSettings::~MapSettings() {
  delete ui;
}

QStringList MapSettings::OgcFilteredOptions = QStringList() << "wms_title" << "wfs_title" << "ows_title"
  << "wms_enable_request" <<  "wfs_enable_request" << "ows_enable_request"
  << "wms_onlineresource" << "wfs_onlineresource" << "ows_onlineresource"
  << "wms_srs" << "wfs_srs" << "ows_srs";


