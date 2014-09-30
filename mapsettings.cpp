#include <QStandardItem>
#include <QStandardItemModel>

#include "mapsettings.h"
#include "ui_mapsettings.h"

MapSettings::MapSettings(QWidget *parent, MapfileParser *mf) :
  QDialog(parent), ui(new Ui::MapSettings), mapfile(mf)
{

    ui->setupUi(this);

    this->mapfile = mf;

    /** Constants **/
    this->units << "inches" << "feet" << "miles" << "meters" << "kilometers" << 
        "dd" << "pixels" << "pourcentages" << "nauticalmiles";

    this->imageTypes << "jpeg" << "pdf" << "png" << "svg";
    this->missingdata << "" << "FAIL" << "LOG" << "IGNORE";

    this->ogcMapOptions << "" << "ows_http_max_age" << "ows_schemas_location" << 
        "ows_sld_enabled" << "ows_updatesequence" << "wms_abstract" <<
        "wms_accessconstraints" << "wms_addresstype" << "wms_address" <<
        "wms_city" << "wms_stateorprovince" << "wms_postcode" << "wms_country" << 
        "wms_attribution_logourl_format" <<
        "wms_attribution_logourl_height" << "wms_attribution_logourl_href" <<
        "wms_attribution_logourl_width" << "wms_attribution_onlineresource" <<
        "wms_attribution_title" << "wms_bbox_extended" <<
        "wms_contactelectronicmailaddress" << "wms_contactfacsimiletelephone" << 
        "wms_contactperson" << "wms_contactorganization" <<
        "wms_contactposition" << "wms_contactvoicetelephone" << "wms_encoding" << 
        "wms_feature_info_mime_type" << "wms_fees" <<
        "wms_getcapabilities_version" << "wms_getlegendgraphic_formatlist" <<
        "wms_getmap_formatlist" << "wms_keywordlist" <<
        "wms_keywordlist_vocabulary" << "wms_keywordlist_[vocabulary name]_items" <<
        "wms_languages" << "wms_layerlimit" << "wms_resx" <<
        "wms_resy" << "wms_rootlayer_abstract" << "wms_rootlayer_keywordlist" << 
        "wms_rootlayer_title" << "wms_service_onlineresource" <<
        "wms_timeformat" << "ows_schemas_location" << "ows_updatesequence" <<
        "wfs_abstract" << "wfs_accessconstraints" << "wfs_encoding" <<
        "wfs_feature_collection" << "wfs_fees" <<
        "wfs_getcapabilities_version" << "wfs_keywordlist" <<
        "wfs_maxfeatures" << "wfs_namespace_prefix" << "wfs_namespace_uri" <<
        "wfs_service_onlineresource";

    /** General Tab **/
    //TODO: create Slots and Signal on extent on update button
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
    ui->mf_map_maxsize->setValue(this->mapfile->getMapSize());
    //Units
    ui->mf_map_units->addItems(this->units);
    ui->mf_map_units->setCurrentIndex(this->mapfile->getMapUnits());

    //Outpuformat
    ui->mf_map_outputformat->addItems(this->imageTypes);
    //TODO: add custom outputformat
    //ui->mf_map_outputformat->setCurrentIndex(this->mapfile->getMapImageTypes());

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
    ui->mf_map_config_contexturl->setText(this->mapfile->getConfigContextUrl());
    ui->mf_map_config_encryption->setText(this->mapfile->getConfigEncryptionKey());

    if (this->mapfile->getConfigNonsquare() != NULL ) {
        ui->mf_map_config_squarepixel_yes->setChecked(true);
        ui->mf_map_config_squarepixel_off->setChecked(false);
    } else {
        ui->mf_map_config_squarepixel_yes->setChecked(false);
        ui->mf_map_config_squarepixel_off->setChecked(true);
    }

    ui->mf_map_config_projlib->setText(this->mapfile->getConfigProjLib());

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

    ui->mf_map_web_md_option_name->addItems(this->ogcMapOptions);
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
        ui->mf_map_debug->setValue(this->mapfile->debug);
        ui->mf_map_debug->setEnabled(false);
    }
    //TODO: add option for relative/absolute debug file in forms.
    //TODO: Test if file exist or form is empty, if not warn user
    this->connect(ui->mf_map_config_errorFile_browse, SIGNAL(clicked()), SLOT(browseDebugFile()));
    ui->mf_map_config_errorFile->setText(this->mapfile->getDebugFile());

    ui->mf_map_config_missingdata->addItems(this->missingdata);
    if (this->mapfile->getConfigMissingData() != NULL ) {
        ui->mf_map_config_missingdata->setCurrentIndex(this->missingdata.lastIndexOf(this->mapfile->getConfigMissingData()));
    }

    // Filling the table by known OGC metadata from the mapfile
    QHash<QString, QString> metadatas = this->mapfile->getMetadatas();
    QStringList ks = metadatas.keys();
    for (int i = 0;  i < ks.size(); ++i) {
        QString key = ks.at(i);
        QString value = metadatas.value(key);
        if (this->ogcMapOptions.contains(key)) {
            this->addConfigOptionsToModel(key, value);
        }
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
/** End refactoring **/

void MapSettings::accept() {
    this->saveMapSettings();

    QDialog::accept();
}

/** End SLOTS **/

MapSettings::~MapSettings() {
  // mapfile lifecycle should be managed elsewhere
  delete ui;
}
