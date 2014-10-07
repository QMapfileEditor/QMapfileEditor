#include <QStandardItem>
#include <QStandardItemModel>

#include "mapsettings.h"
#include "ui_mapsettings.h"

MapSettings::MapSettings(QWidget * parent, MapfileParser  * mf) :
  QDialog(parent), ui(new Ui::MapSettings), mapfile(mf)
{

    ui->setupUi(this);

    this->settingsUndoStack = new QUndoStack(this);

    /** General Tab **/
    //TODO: create Slots and Signal on extent on update button
    //Name
    ui->mf_map_name->setText(this->mapfile->getMapName());
    this->connect(ui->mf_map_name, SIGNAL(editingFinished()), SLOT(changeMapName()));
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

    //Outpuformat
    ui->mf_map_outputformat->addItems(MapfileParser::imageTypes);
    ui->mf_outputformat_driver->addItems(MapfileParser::drivers);
    //TODO: add custom outputformat
    //ui->mf_map_outputformat->setCurrentIndex(this->mapfile->getMapImageTypes());
    this->connect(ui->mf_outputformat_list, SIGNAL(activated(const QModelIndex &)), SLOT(refreshOutputFormatTab(const QModelIndex &)));
    this->connect(ui->mf_outputformat_driver, SIGNAL(currentIndexChanged(const QString &)), SLOT(refreshGdalOgrDriverCombo(const QString &)));


    //Projection
    //TODO: create autocompleter for projection
    //QCompleter *epsgCompleter = new QCompleter(, this);
    ui->mf_map_projection->addItem(QString::number(this->mapfile->getMapProjection()));

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

    /** OGC Standard tab **/
    //connect
    this->connect(ui->mf_ogc_enable, SIGNAL(toggled(bool)), SLOT(enableOgcStandardFrame(bool)));
    this->connect(ui->mf_map_web_md_options_add, SIGNAL(clicked()), SLOT(addNewOgcMetadata()));

    //fill in forms
    ui->mf_map_web_md_wms_title->setText(this->mapfile->getMetadataWmsTitle());
    ui->mf_map_web_md_wfs_title->setText(this->mapfile->getMetadataWfsTitle());
    //ui->mf_map_web_md_wfs_enable_gc->setChecked(this->mapfile->getMetadataWfsEnabledGc());
    ui->mf_map_web_md_wms_onlineresource->setText(this->mapfile->getMetadataWmsOnlineresource());
    ui->mf_map_web_md_wfs_onlineresource->setText(this->mapfile->getMetadataWfsOnlineresource());
    ui->mf_map_web_md_wms_srs->setText(this->mapfile->getMetadataWmsSrs());
    ui->mf_map_web_md_wfs_srs->setText(this->mapfile->getMetadataWfsSrs());

    ui->mf_map_web_md_option_name->addItems(MapfileParser::ogcMapOptions);
    this->createOgcOptionsModel();

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

    // Filling the table by known OGC metadata from the mapfile
    QHash<QString, QString> metadatas = this->mapfile->getMetadatas();
    QStringList ks = metadatas.keys();
    for (int i = 0;  i < ks.size(); ++i) {
        QString key = ks.at(i);
        QString value = metadatas.value(key);
        if (MapfileParser::ogcMapOptions.contains(key)) {
            this->addConfigOptionsToModel(key, value);
        }
    }

    // output formats
    QList<OutputFormat> outputFmtList = this->mapfile->getOutputFormats();
    QStandardItemModel * outputFmtModel = new QStandardItemModel(this->ui->mf_outputformat_list);
    outputFmtModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Format name")));
    this->ui->mf_outputformat_list->setModel(outputFmtModel);
    for (int i = 0 ; i < outputFmtList.size(); ++i) {
      OutputFormat fmt = outputFmtList.at(i);
      QStandardItem * item = new QStandardItem(fmt.getName());
      item->setEditable(false);
      outputFmtModel->appendRow(item);
    }
}

//Methods
void MapSettings::createOgcOptionsModel() {
    QStandardItemModel  * ogcOptions_model = new QStandardItemModel(0, 2);
    QStringList header;
    header << "Name" << "Value";
    ogcOptions_model->setHorizontalHeaderLabels(header);

    // TODO check for memory leak
    // ensures that the model is destroy along with ui objects (see doc)
    ui->mf_map_web_md_options_list->setModel(ogcOptions_model);
}

void MapSettings::saveMapSettings() {
    this->mapfile->setMapName(ui->mf_map_name->text());
    /** General tab **/
    if (ui->mf_map_status_on->isChecked() ) {
      this->mapfile->setMapStatus(1);
    } else if (ui->mf_map_status_off->isChecked() ) {
      this->mapfile->setMapStatus(0);
    }
    this->mapfile->setMapSize(ui->mf_map_size_width->value(), ui->mf_map_size_height->value());
    this->mapfile->setMapMaxsize(ui->mf_map_maxsize->value());
    //units
    this->mapfile->setMapUnits(ui->mf_map_units->currentText());
    //TODO: outputformat
    //this->mapfile->setOutputformat(ui->mf_map_ouputformat->currentText());
    //projection
    this->mapfile->setMapProjection(ui->mf_map_projection->currentText());
    //extent
    this->mapfile->setMapExtent(ui->mf_map_extent_left->text().toFloat(), ui->mf_map_extent_bottom->text().toFloat(),ui->mf_map_extent_right->text().toFloat(),ui->mf_map_extent_top->text().toFloat());
    
    /** Debug tab **/
    if(ui->mf_map_debug_on->isChecked()) {
        this->mapfile->setDebug(ui->mf_map_debug->value());
    } else if (ui->mf_map_debug_off->isChecked()) {
        this->mapfile->setDebug(0);
    }
    this->mapfile->setMetadata("ms_errorfile", ui->mf_map_config_errorFile->text());
    this->mapfile->setMetadata("missingdata", ui->mf_map_config_missingdata->currentText());
    
    /** Path tab **/
    this->mapfile->setShapepath(ui->mf_map_shapepath->text());
    this->mapfile->setSymbolSet(ui->mf_map_symbolset->text());
    this->mapfile->setFontSet(ui->mf_map_fontset->text());

    /** Advanced tab **/
    this->mapfile->setResolution(ui->mf_map_resolution->value());
    this->mapfile->setDefResolution(ui->mf_map_defresolution->value());
    this->mapfile->setAngle(ui->mf_map_angle->value());
    QColor imageColor = ui->mf_map_imagecolor->palette().color(QWidget::backgroundRole());
    this->mapfile->setImageColor(imageColor.red(), imageColor.green(), imageColor.blue());
    this->mapfile->setTemplatePattern(ui->mf_map_templatepattern->text());
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
        //TODO: boucle on custom metadata list
    }


}

//SLOTS
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

void MapSettings::addNewOgcMetadata() {
    QString value = ui->mf_map_web_md_option_value->text();
    QString optionName = ui->mf_map_web_md_option_name->currentText();

    if ((! value.isEmpty()) && (! optionName.isEmpty()) &&
      (! alreadyInModel(optionName))) {

        this->addConfigOptionsToModel(optionName, value);
        ui->mf_map_web_md_option_name->currentText();
        ui->mf_map_web_md_option_value->setText("");
    }
}

bool MapSettings::alreadyInModel(const QString & key) {
  QStandardItemModel * mod = (QStandardItemModel *) ui->mf_map_web_md_options_list->model();
  // should not happen
  if (! mod) return false;
  return (! mod->findItems(key).isEmpty());
}

void MapSettings::addConfigOptionsToModel(const QString & name, const QString & value) {
  QStandardItemModel * mod = (QStandardItemModel *) ui->mf_map_web_md_options_list->model();
  if (! mod) {
    return;
  }

  QList<QStandardItem *> row;

  row << new QStandardItem(name);
  row << new QStandardItem(value);

  mod->appendRow(row);
  ui->mf_map_web_md_options_list->resizeColumnsToContents();
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


void MapSettings::refreshOutputFormatTab(const QModelIndex &i) {
    QStandardItem * item = ((QStandardItemModel *) ui->mf_outputformat_list->model())->itemFromIndex(i);
    if (item != NULL) {
      OutputFormat selFmt = this->mapfile->getOutputFormat(item->text());
      if (! selFmt.isEmpty()) {
        this->ui->mf_outputformat_name->setText(selFmt.getName());
        this->ui->mf_outputformat_transparent_on->setChecked(selFmt.getTransparent());
        this->ui->mf_outputformat_transparent_off->setChecked(! selFmt.getTransparent());

        this->ui->mf_outputformat_extension->setText(selFmt.getExtension());
        this->ui->mf_outputformat_mimetype->setText(selFmt.getMimeType());

        int driIdx = this->ui->mf_outputformat_driver->findText(selFmt.getDriver());
        if (driIdx != -1) this->ui->mf_outputformat_driver->setCurrentIndex(driIdx);

        this->toggleOutputFormatsWidgets(true);
      }
    }
}

void MapSettings::toggleOutputFormatsWidgets(const bool &enable) {
  this->ui->outputFormatForm->setEnabled(enable);
  this->ui->mf_outputformat_form_buttons->setEnabled(enable);
}


void MapSettings::changeMapName() {
  // if the map name has been left unchanged, do nothing
  if (this->mapfile->getMapName() == this->ui->mf_map_name->text())
    return;

  ChangeMapNameCommand * cmd = new ChangeMapNameCommand(this->ui->mf_map_name->text(), this->mapfile);
  settingsUndoStack->push(cmd);
}
/** End SLOTS **/

MapSettings::~MapSettings() {
  // mapfile lifecycle should be managed elsewhere
  delete ui;
}
