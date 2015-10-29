#ifndef CSLIDER_H
#define CSLIDER_H

#include <QGraphicsItemGroup>
#include <QRgb>
#include "crange.h"

class QString;
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsTextItem;

class CSlider : public QGraphicsItemGroup
{
public:
    CSlider(QGraphicsItem *parent = 0);
    ~CSlider();

    void setPurview(const RealRange&);
    const RealRange& getPurview() const;

    void setGeometry(const QRectF&);
    const QRectF& getGeometry() const;

    const RealRange& getRange() const;
    void setRange(const RealRange&);

    void setDragable(const bool);
    bool isDragable() const;

    void setLabelTemplate(const QString&);
    const QString& getLabelTemplate() const;

    QGraphicsPolygonItem* getLabelBar();
    QGraphicsTextItem* getLabelText();
    QGraphicsLineItem* getLine();
    void setColor(const QColor&);
    const QColor &getColor() const;

    qreal getPosition() const;
    void setPosition(const qreal&);
    qreal getPixelPosition() const;
    void setPixelPosition(const qreal&);


    int topLabelHeight() const;
    int bottomLabelheight() const;


    void update();

private:
    ///area in real coordinates where the slider may be dragable
    RealRange __purview;
    RealRange __range;
    ///position on scene
    QRectF __geometry;
    double __position;
    bool __dragable;
    QString __labelTemplate;
    QColor __color;

    QGraphicsPolygonItem* const __labelBar;
    QGraphicsPolygonItem* const __bottomArrow;
    QGraphicsPolygonItem* const __topArrow;
    QGraphicsTextItem* const __labelText;
    QGraphicsLineItem* const __line;
};

class CSliders : public QList<CSlider*> {
    static const int SLIDER_DRAG_RADIUS;
public:
    CSliders() : current(nullptr) {}

    void setCurrent(CSlider* const s) const;
    CSlider* getCurrent() const { return current; }
    CSlider* findNear(const qreal& position) const;
private:
    mutable CSlider* current;
};

#endif // CSLIDER_H
