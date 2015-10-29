#include "cslider.h"

#include <QString>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QBrush>
#include <QPen>

#include "crange.h"

CSlider::CSlider(QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
    , __position(0.0)
    , __dragable(true)
    , __labelTemplate("%1")
    , __labelBar(new QGraphicsPolygonItem(this))
    , __bottomArrow(new QGraphicsPolygonItem(this))
    , __topArrow(new QGraphicsPolygonItem(this))
    , __labelText(new QGraphicsTextItem(this))
    , __line(new QGraphicsLineItem(this))
    , __color(Qt::black)
{
    __bottomArrow->setPolygon(QPolygonF()
                              << QPointF(0.0,0.0)
                              << QPointF(-5.0,10.0)
                              << QPointF(0.0, 7.0)
                              << QPointF(5.0, 10.0));
    __topArrow->setPolygon(QPolygonF()
                           << QPointF(0.0,0.0)
                           << QPointF(-5.0,-10.0)
                           << QPointF(0.0, -7.0)
                           << QPointF(5.0, -10.0));
    __labelText->setPlainText(__labelTemplate.arg(__position));
}

CSlider::~CSlider() {

}

void CSlider::update() {
    if (__geometry.height() <= 0) {
        return;
    }
    const double pPos(getPixelPosition());
    __bottomArrow->setPos(pPos, __geometry.bottom());
    __topArrow->setPos(pPos, __geometry.top());
    __line->setLine(pPos, __geometry.top(), pPos, __geometry.bottom());
    __labelText->setPos(pPos - __labelText->boundingRect().width() / 2.0, __geometry.top() - topLabelHeight());
}

int CSlider::topLabelHeight() const {
    return 15 + __labelText->boundingRect().height();
}

int CSlider::bottomLabelheight() const {
    return 0;
}

void CSlider::setColor(const QColor& c) {
    __color = c;
    auto l(QList<QAbstractGraphicsShapeItem*>() << __labelBar << __bottomArrow << __topArrow);
    for (QAbstractGraphicsShapeItem* i : l) {
        QPen p(i->pen());
        p.setColor(c);
        i->setPen(c);
    }
}

const QColor& CSlider::getColor() const {
    return __color;
}

void CSlider::setGeometry(const QRectF& r) {
    __geometry = r;
}

const QRectF& CSlider::getGeometry() const {
    return __geometry;
}

void CSlider::setPosition(const double& p) {
    __position = p;
    __labelText->setPlainText(__labelTemplate.arg(__position));
}

void CSlider::setPurview(const RealRange& p) {
    __purview = p;
    //to retest the position
    setPosition(__position);
}

qreal CSlider::getPosition() const {
    return __position;
}

qreal CSlider::getPixelPosition() const {
    return __range[__position] * __geometry.width() + __geometry.x();
}

void CSlider::setPixelPosition(const qreal& v) {
    setPosition(std::min(std::max(__range((v - __geometry.x()) / __geometry.width()), __purview.getMin()), __purview.getMax()));
}

const RealRange& CSlider::getPurview() const {
    return __purview;
}

void CSlider::setRange(const RealRange& d) {
    __range = d;
}

const RealRange& CSlider::getRange() const {
    return __range;
}

void CSlider::setDragable(bool v) {
    __dragable = v;
}

bool CSlider::isDragable() const {
    return __dragable;
}


void CSlider::setLabelTemplate(const QString& s) {
    __labelTemplate = s;
    __labelText->setPlainText(__labelTemplate.arg(__position));
}
const QString& CSlider::getLabelTemplate() const { return __labelTemplate; }
QGraphicsPolygonItem* CSlider::getLabelBar() { return __labelBar; }
QGraphicsTextItem* CSlider::getLabelText() { return __labelText; }
QGraphicsLineItem* CSlider::getLine() { return __line; }

const int CSliders::SLIDER_DRAG_RADIUS = 3;

void CSliders::setCurrent(CSlider* const s) const {
    Q_ASSERT(this->contains(s) || s == nullptr); current - s;
}

CSlider* CSliders::findNear(const qreal& x) const {
    for (CSlider* s : *this) {
        if (s->isDragable() && abs(s->getPixelPosition() - x) < SLIDER_DRAG_RADIUS) {
            return s;
        }
    }
    return nullptr;
}

