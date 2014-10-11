#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class MapScene : public QGraphicsScene {

 Q_OBJECT

 public:
  MapScene(QObject * parent = 0);
  ~MapScene();

  void setZoomMode(bool const & v);

 signals:
  void notifyAreaToZoom(QRectF);

 protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

 private:
  QGraphicsRectItem * zoomInArea;
  bool drawing;
  bool zoomMode;
  QPointF pointOrig;

};


#endif // MAPSCENE_H
