#include "c2dchart.h"
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QOpenGLContext>
#include <QSizeF>

namespace CGL {

const QFont C2dChart::TITLE_FONT = QFont("Tahoma", 20);
const QFont C2dChart::LABELS_FONT = QFont("Tahoma", 14);
const QFont C2dChart::TICKETS_FONT = QFont("Tahoma", 11);

C2dChart::C2dChart(QWidget* parent)
    : QGraphicsView(parent)
{
    this->setScene(new QGraphicsScene(this));
    title = scene()->addSimpleText(QString(), TITLE_FONT);
    xLabel = scene()->addSimpleText(QString(), LABELS_FONT);
    yLabel = scene()->addSimpleText(QString(), LABELS_FONT);
    yLabel->setRotation(-90);
    chart = new C2dChartPlace(this);
    chart->setGridStep(C2dChartAxis::commendableTicketSize(TICKETS_FONT, this).width() * 2.0);
    this->scene()->addItem(yAxis = new C2dChartAxis(Qt::LeftEdge, 0));
    this->scene()->addItem(xAxis = new C2dChartAxis(Qt::BottomEdge, 0));

    this->setScene(scene());
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

C2dChart::~C2dChart()
{

}

void C2dChart::setData(const CChartData& newData) {
    data.clear();
    addData(newData);
}

void C2dChart::setData(const CChartData::ChartDataList& newData) {
    data.clear();
    addData(newData);
}

void C2dChart::addData(const CChartData& newData) {
    Q_ASSERT(newData.size() == 2);
    Q_ASSERT(newData[0]->size() == newData[1]->size());
    data.push_back(newData);
    if (title->text().isNull()) {
        title->setText(newData.getTitle());
    }
    if (xLabel->text().isNull()) {
        xLabel->setText(newData[0]->getLabel());
    }
    if (yLabel->text().isNull()) {
        yLabel->setText(newData[1]->getLabel());
    }
    chart->setData(data);
}

void C2dChart::addData(const CChartData::ChartDataList &d) {
    for (const CChartData& newData: d) {
        Q_ASSERT(newData.size() == 2);
        Q_ASSERT(newData[0]->size() == newData[1]->size());
        data.push_back(newData);
        if (title->text().isNull()) {
            title->setText(newData.getTitle());
        }
        if (xLabel->text().isNull()) {
            xLabel->setText(newData[0]->getLabel());
        }
        if (yLabel->text().isNull()) {
            yLabel->setText(newData[1]->getLabel());
        }
    }
    chart->setData(data);
}

void C2dChart::setTitle(const QString& val) {
    title->setText(val);
}

void C2dChart::setXLabel(const QString& val) {
    xLabel->setText(val);

}

void C2dChart::setYLabel(const QString& val) {
    yLabel->setText(val);
}

QString C2dChart::getTitle() const {
    return title->text();
}
QString C2dChart::getXLabel() const {
    return xLabel->text();
}
QString C2dChart::getYLabel() const {
    return yLabel->text();
}

void C2dChart::resizeEvent(QResizeEvent*) {
    QMarginsF margins(0.0, 0.0, 2.0, 0.0);
    scene()->setSceneRect(QRectF(QPointF(0.0, 0.0), QSizeF(this->size())));

    if (!title->text().isEmpty()) {
        title->show();
        //to estimate position of title need to know width of yLabel
        margins += QMarginsF(0.0, title->boundingRect().height(), 0.0, 0.0);
    } else {
        title->hide();
    }

    if (!xLabel->text().isEmpty()) {
        xLabel->show();
        margins += QMarginsF(0.0, 0.0, 0.0, xLabel->boundingRect().height());
    } else {
        xLabel->hide();
    }

    if (!yLabel->text().isEmpty()) {
        yLabel->show();
        margins += QMarginsF(yLabel->boundingRect().height(), 0.0, 0.0, 0.0);
    } else {
        yLabel->hide();
    }

    margins += QMarginsF(0.0, 0.0, 0.0, C2dChartAxis::commendableAxisWeight(xAxis->getFont(), this));
    qreal centerH(scene()->height() - margins.bottom() - margins.top());
    yAxis->setLength(centerH);
    yAxis->setRange(RealRange(chart->getViewPort().bottom(), chart->getViewPort().top()));
    margins += QMarginsF(yAxis->implementation(QPointF(margins.left(), margins.top()),
                                               chart->yGridInterval(centerH)),
                         0.0, 0.0, 0.0);
    qreal centerW(scene()->width() - margins.left() - margins.right());
    xAxis->setLength(centerW);
    xAxis->setRange(RealRange(chart->getViewPort().left(), chart->getViewPort().right()));
    xAxis->implementation(QPointF(margins.left(), margins.top() + centerH), chart->xGridInterval(centerW));

    chart->move(margins.left(), margins.top());
    chart->resize(centerW, centerH);

    title->setPos(scene()->width() / 2.0 - title->boundingRect().width() / 2.0, 0);

    xLabel->setPos(margins.left() + chart->width() / 2.0 - xLabel->boundingRect().width() / 2.0,
                   scene()->height() - xLabel->boundingRect().height());

    yLabel->setPos(0, scene()->height() - margins.bottom() - chart->height() / 2.0 + yLabel->boundingRect().width() / 2.0);
}

}
