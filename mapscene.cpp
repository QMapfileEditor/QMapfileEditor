/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: 
 * Author: Pierre Mauduit
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

#include "mapscene.h"

#include <cmath>

MapScene::MapScene(QObject *parent): QGraphicsScene(parent),
    zoomInArea(0),
    drawing(false),
    zoomingIn(false),
    zoomingOut(false),
    panning(false)
{}


MapScene::~MapScene() {
};

void MapScene::setZoomingIn(bool const &b) { zoomingIn = b; }
void MapScene::setZoomingOut(bool const &b) { zoomingOut = b; }
void MapScene::setPanning(bool const &b) { panning = b; }

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() != Qt::LeftButton)
    return;
  if (zoomingIn) {
    pointOrig = QPointF(event->scenePos().x(), event->scenePos().y());
    zoomInArea = new QGraphicsRectItem(pointOrig.x(), pointOrig.y(), 1, 1);
    zoomInArea->setBrush(QBrush(QColor(24, 148, 24, 40)));
    zoomInArea->setPen(QPen(QColor(24, 148, 24), 1.5));
    addItem(zoomInArea);
    drawing = true;
  }
  else if (panning) {
    pointOrig = QPointF(event->scenePos().x(), event->scenePos().y());
  }
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (drawing) {
      qreal newx  = event->scenePos().x();
      qreal newy = event->scenePos().y();

      qreal nWidth = std::abs(newx - pointOrig.x());
      qreal nHeight = std::abs(newy - pointOrig.y());
      zoomInArea->setRect(newx > pointOrig.x() ? pointOrig.x() : newx,
                          newy > pointOrig.y() ? pointOrig.y() : newy,
                          nWidth, nHeight);
  }
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() != Qt::LeftButton)
    return;
  if ((drawing) && (zoomingIn)) {
    emit notifyAreaToZoomIn(zoomInArea->rect());
    // After having emitted the signal, the scene will be cleared
    zoomInArea = NULL;
    drawing = false;
  } else if (zoomingOut) {
    emit notifyAreaToZoomOut();
  } else if (panning) {
    qreal relatx = event->scenePos().x() - pointOrig.x();
    qreal relaty = event->scenePos().y() - pointOrig.y();

    emit notifyAreaToPan(relatx, relaty);
  }
}
