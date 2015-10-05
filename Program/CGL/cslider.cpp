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
    , __labelBar(new QGraphicsPolygonItem(this))
    , __bottomArrow(new QGraphicsPolygonItem(this))
    , __topArrow(new QGraphicsPolygonItem(this))
    , __labelText(new QGraphicsTextItem(this))
    , __line(new QGraphicsLineItem(this))
{
    QBrush b(__labelBar->brush());
    b.setColor(Qt::blue);
    QPen p(__labelBar->pen());
    p.setColor(Qt::red);
    __labelBar->setBrush(b);
    __labelBar->setPen(p);
    __line->setPen(p);

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
}

CSlider::~CSlider() {

}

void CSlider::update() {
    if (__geometry.height() <= 0) {
        return;
    }
    qreal pos(5);
    __bottomArrow->setPos(pos, __geometry.bottom());
    __topArrow->setPos(pos, __geometry.top());
    __line->setLine(pos, __geometry.top() - 10, pos, __geometry.bottom());
}

int CSlider::topLabelHeight() const { return 40; }
int CSlider::bottomLabelheight() const { return 0; }

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
}

double CSlider::getPosition() const {
    return __position;
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

void CSlider::setLabelTemplate(const QString& s) { __labelTemplate = s; }
const QString& CSlider::getLabelTemplate() const { return __labelTemplate; }
QGraphicsPolygonItem* CSlider::getLabelBar() { return __labelBar; }
QGraphicsTextItem* CSlider::getLabelText() { return __labelText; }
QGraphicsLineItem* CSlider::getLine() { return __line; }

