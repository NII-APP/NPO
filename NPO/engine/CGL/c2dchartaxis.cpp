#include "c2dchartaxis.h"
#include <QFontMetricsF>
#include <QSizeF>
#include "cscale.h"

namespace CGL {


const qreal C2dChartAxis::Ticket::TAIL_LENGHT = 5.0;
const qreal C2dChartAxis::Ticket::TAIL_MARGIN = 2.0;

C2dChartAxis::C2dChartAxis(Qt::Edge side, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
{
    edge = side;
    border = new QGraphicsLineItem(this);
}

C2dChartAxis::~C2dChartAxis()
{

}

QSizeF C2dChartAxis::commendableTicketSize(const QFont& font, QPaintDevice* paintdevice)
{
    return QFontMetricsF(font, paintdevice).size(Qt::TextSingleLine, "9.99E-99");
}

qreal C2dChartAxis::commendableAxisWeight(const QFont& font, QPaintDevice* paintdevice)
{
    return commendableTicketSize(font, paintdevice).height() + Ticket::TAIL_LENGHT + Ticket::TAIL_MARGIN;
}

qreal C2dChartAxis::implementation(const QPointF& topleft, const CInterval& tickValues, QPaintDevice *forMetric)
{
    RealScale scale(range, domain);
    this->setPos(topleft);
    if (tickets.size() > tickValues.size()) {
    }
    while (tickets.size() < tickValues.size()) {
        tickets.push_back(new Ticket(edge, this));
        tickets.last()->show();
    }
    for (int i(0); i != tickValues.size(); ++i) {
        tickets[i]->show();
    }
    for (int i(tickValues.size()); i != tickets.size(); ++i) {
        tickets[i]->hide();
    }

    qreal maxWidth(0.0);
    for (int i(0); i != tickValues.size(); ++i) {
        tickets[i]->setText(QString::number(tickValues[i], 'g', 3));
        maxWidth = std::max(tickets[i]->boundingRect().width(), maxWidth);
    }
    for (int i(0); i != tickValues.size(); ++i) {
        if (edge & (Qt::LeftEdge | Qt::RightEdge)) {
            tickets[i]->setLocation(QPointF(maxWidth * (edge == Qt::LeftEdge), scale(tickValues[i])));
        } else {
            tickets[i]->setLocation(QPointF(scale(tickValues[i]),
     (commendableTicketSize(font, forMetric).height() + Ticket::TAIL_MARGIN + Ticket::TAIL_LENGHT) * (edge == Qt::TopEdge)));
        }
    }

    switch (edge) {
    case Qt::LeftEdge:
        border->setLine(maxWidth - 1, domain.getMax(), maxWidth - 1, domain.getMin());
        break;
    case Qt::RightEdge:
        border->setLine(-1, domain.getMax(), -1, domain.getMin());
        break;
    case Qt::BottomEdge:
        border->setLine(domain.getMax(), -1, domain.getMin(), -1);
        break;
    case Qt::TopEdge:
        border->setLine(domain.getMax(), commendableTicketSize(font, forMetric).height() + Ticket::TAIL_MARGIN + Ticket::TAIL_LENGHT,
                        domain.getMin(), commendableTicketSize(font, forMetric).height() + Ticket::TAIL_MARGIN + Ticket::TAIL_LENGHT);
        qDebug() << "line" << border->line();
        break;
    }

    return maxWidth + 1;
}


C2dChartAxis::Ticket::Ticket(Qt::Edge side, QGraphicsItem* parent)
    : QGraphicsItemGroup(parent)
{
    edge = side;
    text = new QGraphicsSimpleTextItem(this);
    tail = new QGraphicsLineItem(this);
}

void C2dChartAxis::Ticket::setText(const QString& value) {
    text->setText(value);
    refreshPosition();
}

void C2dChartAxis::Ticket::setLocation(const QPointF& locate)
{
    const QRectF txt(text->boundingRect());
    switch (edge) {
    case Qt::LeftEdge:
        this->setPos(locate.x() - txt.width() - TAIL_MARGIN - TAIL_LENGHT,
                     locate.y() - txt.height() / 2);
        text->setPos(0,0);
        tail->setLine(txt.width() + TAIL_MARGIN, txt.height() / 2,
                      txt.width() + TAIL_MARGIN + TAIL_LENGHT, txt.height() / 2);
        break;
    case Qt::RightEdge:
        this->setPos(locate.x(), locate.y() - txt.height() / 2);
        text->setPos(TAIL_MARGIN + TAIL_LENGHT,0);
        tail->setLine(0, txt.height() / 2, TAIL_LENGHT, txt.height() / 2);
        break;
    case Qt::BottomEdge:
        this->setPos(locate.x() - txt.width() / 2.0, locate.y());
        text->setPos(0, TAIL_MARGIN + TAIL_LENGHT);
        tail->setLine(txt.width() / 2.0, 0, txt.width() / 2.0, TAIL_LENGHT);
        break;
    case Qt::TopEdge:
        this->setPos(locate.x() - txt.width() / 2.0, locate.y() - txt.height() - TAIL_MARGIN - TAIL_LENGHT);
        text->setPos(0,0);
        tail->setLine(txt.width() / 2.0, txt.height() + TAIL_MARGIN + TAIL_LENGHT, txt.width() / 2.0, txt.height() + TAIL_MARGIN);
        break;
    }
}

void C2dChartAxis::Ticket::setFont(const QFont& value)
{
    text->setFont(value);
    refreshPosition();
}

void C2dChartAxis::Ticket::refreshPosition()
{
    setLocation(tail->line().p2());
}

void C2dChartAxis::setFont(const QFont& val)
{
    font = val;
    for (Ticket* ticket : tickets) {
        ticket->setFont(val);
    }
}

QRectF C2dChartAxis::Ticket::boundingRect() const {
    return QRectF(this->pos(), QSizeF(text->boundingRect().width() + (TAIL_LENGHT + TAIL_MARGIN) * +!!(edge & (Qt::LeftEdge | Qt::RightEdge)),
                                      text->boundingRect().height() + (TAIL_LENGHT + TAIL_MARGIN) * +!!(edge & (Qt::TopEdge | Qt::BottomEdge))));
}
}
