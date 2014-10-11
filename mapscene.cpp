#include "mapscene.h"

#include <cmath>

MapScene::MapScene(QObject *parent): QGraphicsScene(parent),
    zoomInArea(0),
    drawing(false),
    zoomMode(false)
{}


MapScene::~MapScene() {
};

void MapScene::setZoomMode(bool const &b) { zoomMode = b; }

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() != Qt::LeftButton)
    return;
  if (zoomMode) {
    pointOrig = QPointF(event->scenePos().x(), event->scenePos().y());
    zoomInArea = new QGraphicsRectItem(pointOrig.x(), pointOrig.y(), 1, 1);
    zoomInArea->setBrush(QBrush(QColor(24, 148, 24, 40)));
    zoomInArea->setPen(QPen(QColor(24, 148, 24), 1.5));
    addItem(zoomInArea);
    drawing = true;
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
  if (drawing) {
    removeItem(zoomInArea);
    delete zoomInArea;
    zoomInArea = NULL;
    drawing = false;
    // TODO: triggers onto mainwindow the order to redraw the map
  }
}
