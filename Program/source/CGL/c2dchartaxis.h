#ifndef C2DCHARTAXIS_H
#define C2DCHARTAXIS_H

#include <QGraphicsItemGroup>
#include <QFont>
#include <QtGlobal>
#include "crange.h"
#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>
#include "cinterval.h"


class C2dChartAxis : public QGraphicsItemGroup
{
    Qt::Edge edge;
    QFont font;
    CRealRange range;
    CRealRange domain;
    QGraphicsLineItem* border;

    class Ticket;

    typedef QVector<Ticket*> Tickets;
    Tickets tickets;

public:
    C2dChartAxis(Qt::Edge side, QGraphicsItem* parent = 0);
    ~C2dChartAxis();

    void setLength(const qreal val) { domain = edge & (Qt::LeftEdge | Qt::RightEdge) ? CRealRange(val - 2, -1) : CRealRange(-1, val - 1); }
    void setDomain(const CRealRange& val) { domain = val; }
    const CRealRange& getDomain() const { return domain; }
    void setRange(const CRealRange& val) { range = val; }
    const CRealRange& getRange() const { return range; }
    void setFont(const QFont& val);
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


#endif // C2DCHARTAXIS_H
