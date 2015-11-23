#ifndef CSLIDER_H
#define CSLIDER_H

#include <QGraphicsItemGroup>
#include <QRgb>
#include "crange.h"

class QString;
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsTextItem;
class QGraphicsProxyWidget;

class CSlider : public QGraphicsItemGroup
{
public:
    explicit CSlider(QGraphicsItem *parent = 0);
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

    QGraphicsProxyWidget* getLabelBar();
    QGraphicsLineItem* getLine();

    qreal getPosition() const;
    void setPosition(const qreal&);
    qreal getPixelPosition() const;
    void setPixelPosition(const qreal&);

    void setProxyLabel(QWidget*);

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

    QGraphicsProxyWidget* const __labelBar;
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
