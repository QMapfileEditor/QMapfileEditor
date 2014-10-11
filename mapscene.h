#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class MapScene : public QGraphicsScene {

 public:
  MapScene(QObject * parent = 0);
  ~MapScene();

  void setZoomMode(bool const & v);

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
