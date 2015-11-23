#include "cslider.h"

#include <QString>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QBrush>
#include <QPen>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QPalette>

#include "crange.h"

CSlider::CSlider(QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
    , __position(0.0)
    , __dragable(true)
    , __labelTemplate("%1")
    , __labelBar(new QGraphicsProxyWidget(this))
    , __line(new QGraphicsLineItem(this))
{
    __labelBar->setWidget(new QLabel(0));
    QPalette p(__labelBar->widget()->palette());
    p.setBrush(QPalette::Background, Qt::white);
    __labelBar->widget()->setPalette(p);
}

CSlider::~CSlider() {

}

void CSlider::update()
{
    if (__geometry.height() <= 0) {
        return;
    }
    const double pPos(getPixelPosition());
    if (pPos != pPos) {
        return;
    }
    __line->setLine(pPos, __geometry.top(), pPos, __geometry.bottom());
    __labelBar->setPos(pPos - (__labelBar->widget()->width() >> 1), __geometry.top() - __labelBar->widget()->height());
}

int CSlider::topLabelHeight() const
{
    return __labelBar->widget()->height();
}

int CSlider::bottomLabelheight() const
{
    return 0;
}

void CSlider::setGeometry(const QRectF& r)
{
    __geometry = r;
}

const QRectF& CSlider::getGeometry() const
{
    return __geometry;
}

void CSlider::setPosition(const double& p)
{
    if (__purview.getMin() < __purview.getMax()) {
        __position = std::min(std::max(p, __purview.getMin()), __purview.getMax());
    } else {
        __position = p;
    }
    if (dynamic_cast<QLabel*>(__labelBar->widget())) {
        static_cast<QLabel*>(__labelBar->widget())->setText(__labelTemplate.arg(__position));
        __labelBar->resize(__labelBar->widget()->sizeHint());
        __labelBar->resize(__labelBar->widget()->size());
    }
    update();
}

void CSlider::setProxyLabel(QWidget* w)
{
    __labelBar->setWidget(w);
}

void CSlider::setPurview(const RealRange& p)
{
    __purview = p;
    //to retest the position
    setPosition(__position);
}

qreal CSlider::getPosition() const
{
    return __position;
}

qreal CSlider::getPixelPosition() const
{
    return __range[__position] * __geometry.width() + __geometry.x();
}

void CSlider::setPixelPosition(const qreal& v)
{
    setPosition(__range((v - __geometry.x()) / __geometry.width()));
}

const RealRange& CSlider::getPurview() const
{
    return __purview;
}

void CSlider::setRange(const RealRange& d)
{
    __range = d;
}

const RealRange& CSlider::getRange() const
{
    return __range;
}

void CSlider::setDragable(bool v)
{
    __dragable = v;
}

bool CSlider::isDragable() const
{
    return __dragable;
}


void CSlider::setLabelTemplate(const QString& s)
{
    __labelTemplate = s;
    if (dynamic_cast<QLabel*>(__labelBar->widget())) {
        static_cast<QLabel*>(__labelBar->widget())->setText(__labelTemplate.arg(__position));
    }
}
const QString& CSlider::getLabelTemplate() const { return __labelTemplate; }
QGraphicsProxyWidget* CSlider::getLabelBar() { return __labelBar; }
QGraphicsLineItem* CSlider::getLine() { return __line; }

const int CSliders::SLIDER_DRAG_RADIUS = 3;

void CSliders::setCurrent(CSlider* const s) const
{
    Q_ASSERT(this->contains(s) || s == nullptr);
    current = s;
}

CSlider* CSliders::findNear(const qreal& x) const
{
    for (CSlider* s : *this) {
        if (s->isDragable() && abs(s->getPixelPosition() - x) < SLIDER_DRAG_RADIUS) {
            return s;
        }
    }
    return nullptr;
}

