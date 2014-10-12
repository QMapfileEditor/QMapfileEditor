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

  void setZoomingIn(bool const & v);
  void setZoomingOut(bool const & v);
  void setPanning(bool const & v);

 signals:
  void notifyAreaToZoomIn(QRectF);
  void notifyAreaToZoomOut();

 protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

 private:
  QGraphicsRectItem * zoomInArea;
  bool drawing;

  bool zoomingIn;
  bool zoomingOut;
  bool panning;

  QPointF pointOrig;

};


#endif // MAPSCENE_H
