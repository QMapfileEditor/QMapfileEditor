/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: Settings of Vector Layer Object
 * Author: Pierre Mauduit / Yves Jacolin
 *
 **********************************************************************
 * Copyright (c) 2014, Yves Jacolin
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

#include "layersettingsvector.h"
#include "ui_layersettingsvector.h"

LayerSettingsVector::LayerSettingsVector(QWidget * parent, MapfileParser * mf, Layer * l) :
  QTabWidget(parent), ui(new Ui::LayerSettingsVector), mapfile(mf), layer(l)
{
    ui->setupUi(this);

    
    /** Layer Tab **/
    ui->mf_layerName_value->setText(l->getName());
    if( l->getStatus() == 0 )
    {
      ui->mf_statusOn_radio->setChecked(true);
      ui->mf_statusOff_radio->setChecked(false);
      ui->mf_statusDefault_radio->setChecked(false);
    } else if(l->getStatus() == 2 ) {
      ui->mf_statusOn_radio->setChecked(false);
      ui->mf_statusOff_radio->setChecked(false);
      ui->mf_statusDefault_radio->setChecked(true);
    } else {
      ui->mf_statusOn_radio->setChecked(false);
      ui->mf_statusOff_radio->setChecked(true);
      ui->mf_statusDefault_radio->setChecked(false);
    }
    
    //TODO: need to fill dropdown list with current layer for mask and requires:
    ui->mf_requires_box->addItem( l->getRequires() );
    ui->mf_requires_box->setCurrentIndex(ui->mf_requires_box->findText(l->getRequires()));
    
    ui->mf_mask_box->addItem( l->getMask() );
    ui->mf_mask_box->setCurrentIndex(ui->mf_mask_box->findText(l->getMask()));
    
    ui->mf_group_edit->setText( l->getGroup() );
    
    ui->mf_opacity_box->setValue( l->getOpacity() );
    
    ui->mf_tolerance_box->setValue( l->getTolerance() );
    //BUG: casse est importante?
    ui->mf_toleranceUnit_combo->setCurrentIndex(ui->mf_toleranceUnit_combo->findText(l->getToleranceUnit()) );
    
    if (l->getMinScale())
      ui->mf_minScaleDenom_edit->setText( QString::number(l->getMinScale()) );
    if( l->getMaxScale())
      ui->mf_maxScaleDenom_edit->setText( QString::number(l->getMaxScale()) );
    
    ui->mf_debug_value->setValue( l->getDebugLevel() );
    
    ui->mf_template_edit->setText( l->getTemplate() );
    ui->mf_footer_value->setText( l->getFooter() );
    ui->mf_header_value->setText( l->getHeader() );
    
    /** Data source Tab **/
    ui->mf_type_combo->setCurrentIndex(ui->mf_type_combo->findText(l->getType()) );
    ui->mf_units_combo->setCurrentIndex(ui->mf_units_combo->findText(l->getUnits()) );
    
    ui->mf_extent_minx->setText( QString::number(l->getMinX()) );
    ui->mf_extent_miny->setText( QString::number(l->getMinY()) );
    ui->mf_extent_maxx->setText( QString::number(l->getMaxX()) );
    ui->mf_extent_maxy->setText( QString::number(l->getMaxY()) );
    
//     ui->mf_filter_edit->setText( l->getFilter() );
}


//SLOTS
void LayerSettingsVector::accept() {
    //this->saveLayerSettings();

    //QDialog::accept();
}
/** End SLOTS **/

LayerSettingsVector::~LayerSettingsVector() {
  delete ui;
}
