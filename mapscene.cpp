#include "mapscene.h"

#include <cmath>

MapScene::MapScene(QObject *parent): QGraphicsScene(parent),
    zoomInArea(0),
    drawing(false)
{}


MapScene::~MapScene() {
};

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
   zoomInArea = new QGraphicsRectItem(event->scenePos().x(), event->scenePos().y(), 1, 1);
   addItem(zoomInArea);
   drawing = true;
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (drawing) {
      qreal origx = zoomInArea->rect().x();
      qreal origy = zoomInArea->rect().y();
      qreal newx  = event->scenePos().x();
      qreal newy = event->scenePos().y();
      qreal nWidth = std::abs(newx - origx);
      qreal nHeight = std::abs(newy - origy);
      zoomInArea->setRect(origx <= newx ? origx : newx, origy <= newy ? origy : newy,
                          nWidth, nHeight);



  }
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  removeItem(zoomInArea);
  delete zoomInArea;
  zoomInArea = NULL;
  drawing = false;
}
