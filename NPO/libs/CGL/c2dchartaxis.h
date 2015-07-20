#ifndef C2DCHARTAXIS_H
#define C2DCHARTAXIS_H

#include <QGraphicsItemGroup>
#include <QFont>
#include <QtGlobal>
#include "crange.h"
#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>
#include "cinterval.h"

namespace CGL {

class C2dChartAxis : public QGraphicsItemGroup
{
    Qt::Edge edge;
    QFont font;
    RealRange range;
    RealRange domain;
    QGraphicsLineItem* border;

    class Ticket;

    typedef QVector<Ticket*> Tickets;
    Tickets tickets;

public:
    C2dChartAxis(Qt::Edge side, QGraphicsItem* parent = 0);
    ~C2dChartAxis();

    void setLength(const qreal val) { domain = edge & (Qt::LeftEdge | Qt::RightEdge) ? RealRange(val - 2, -1) : RealRange(-1, val - 1); }
    void setDomain(const RealRange& val) { domain = val; }
    void setRange(const RealRange& val) { range = val; }
    void setFont(const QFont& val);
    const RealRange& getRange() const { return range; }
    const QFont& getFont() const { return font; }

    qreal implementation(const QPointF& topleft, const CInterval& tickValues, QPaintDevice* forMetric = 0);

    static qreal commendableAxisWeight(const QFont& font, QPaintDevice* paintdevice);
    static QSizeF commendableTicketSize(const QFont& font, QPaintDevice* paintdevice);
};

class C2dChartAxis::Ticket : public QGraphicsItemGroup {
    Qt::Edge edge;
    QGraphicsSimpleTextItem* text;
    QGraphicsLineItem* tail;

    void refreshPosition();
public:
    Ticket(Qt::Edge side, QGraphicsItem* parent = 0);
    ~Ticket() {}

    void setLocation(const QPointF& locate);
    void setText(const QString& value);
    void setFont(const QFont& value);

    static const qreal TAIL_LENGHT;
    static const qreal TAIL_MARGIN;

    virtual QRectF 	boundingRect() const;
};

}

#endif // C2DCHARTAXIS_H
