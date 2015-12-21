#include "c2dchart.h"
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QOpenGLContext>
#include <QSizeF>
#include <QEventLoop>
#include <QCoreApplication>
#include "cdimensioninterval.h"
#include "cdimensionarray.h"
#include "cchartdatalist.h"
#include "cslider.h"

const QFont C2dChart::TITLE_FONT = QFont("Tahoma", 20);
const QFont C2dChart::LABELS_FONT = QFont("Tahoma", 14);
const QFont C2dChart::TICKETS_FONT = QFont("Tahoma", 11);

C2dChart::C2dChart(QWidget* parent)
    : QGraphicsView(parent)
    , haulage(nullptr)
{
    this->setScene(new QGraphicsScene(this));
    title = scene()->addSimpleText(QString(), TITLE_FONT);
    xLabel = scene()->addSimpleText(QString(), LABELS_FONT);
    yLabel = scene()->addSimpleText(QString(), LABELS_FONT);
    yLabel->setRotation(-90);
    chart = new C2dChartPlace(sliders, haulage, this);
    chart->setGridStep(C2dChartAxis::commendableTicketSize(TICKETS_FONT, this).width() * 2.0);
    this->connect(chart, SIGNAL(viewPortChanged(QRectF)), SLOT(update()));
    this->scene()->addItem(yAxis = new C2dChartAxis(Qt::LeftEdge, 0));
    this->scene()->addItem(xAxis = new C2dChartAxis(Qt::BottomEdge, 0));

    this->setScene(scene());
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->connect(chart, SIGNAL(sliderMoved(CSlider*)), SLOT(emitSliderMoved(CSlider*)));
}

C2dChart::~C2dChart()
{
}

void C2dChart::setData(const CChartData& newData) {
    data.clear();
    addData(newData);
}

void C2dChart::setData(const CChartDataList& newData) {
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

void C2dChart::addData(const CChartDataList &d) {
    if (!d.getChartTitle().isNull()) {
        title->setText(d.getChartTitle());
    }
    if (!d.getDimensionTitle(0).isNull()) {
        xLabel->setText(d.getDimensionTitle(0));
    }
    if (!d.getDimensionTitle(1).isNull()) {
        yLabel->setText(d.getDimensionTitle(1));
    }
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

void C2dChart::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);
    update();
}

void C2dChart::update() {
    this->setBackgroundBrush(this->backgroundBrush());
    invalidateScene(QRect(QPoint(0,0), this->size()));
    QMarginsF margins(0.0, 0.0, 2.0, 0.0);
    scene()->setSceneRect(QRectF(QPointF(0.0, 0.0), QSizeF(this->size())));

    if (!title->text().isEmpty()) {
        title->show();
        //to estimate position of title need to know width of yLabel
        margins += QMarginsF(0.0, title->boundingRect().height(), 0.0, 0.0);
    } else {
        title->hide();
    }

    if (!sliders.empty()) {
        margins += QMarginsF(0.0,
                             (**std::max_element(sliders.begin(), sliders.end(),[](const CSlider* s1, const CSlider* s2)->bool
        { return s1->topLabelHeight() > s2->topLabelHeight(); } )).topLabelHeight(),
                             0.0,
                             0.0);
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
    yAxis->setRange(CRealRange(chart->getViewPort().bottom(), chart->getViewPort().top()));
    static const qreal LEFT_SIDE_LABELS_CAPACITY(50);
    const qreal yAxysWidth(yAxis->implementation(QPointF(margins.left(), margins.top()),
                                                    chart->yGridInterval(centerH)));
    const qreal yAxisMargin(std::max(yAxysWidth, LEFT_SIDE_LABELS_CAPACITY));
    yAxis->setX(yAxisMargin - yAxysWidth + margins.left());
    margins += QMarginsF(yAxisMargin,
                         0.0, 0.0, 0.0);
    qreal centerW(scene()->width() - margins.left() - margins.right());
    xAxis->setLength(centerW);
    xAxis->setRange(CRealRange(chart->getViewPort().left(), chart->getViewPort().right()));
    xAxis->implementation(QPointF(margins.left(), margins.top() + centerH), chart->xGridInterval(centerW));

    chart->move(margins.left(), margins.top());
    chart->resize(centerW, centerH);

    title->setPos(scene()->width() / 2.0 - title->boundingRect().width() / 2.0, 0);

    xLabel->setPos(margins.left() + chart->width() / 2.0 - xLabel->boundingRect().width() / 2.0,
                   scene()->height() - xLabel->boundingRect().height());

    yLabel->setPos(0, scene()->height() - margins.bottom() - chart->height() / 2.0 + yLabel->boundingRect().width() / 2.0);

    for (CSlider* s : sliders) {
        s->setRange(xAxis->getRange());
        s->setGeometry(chart->geometry());
        s->update();
    }
    chart->repaint();
}


void C2dChart::closeEvent(QCloseEvent *) {
    emit closed();
}

void C2dChart::mouseMoveEvent(QMouseEvent* event) {
    const auto x(event->x());
    if (event->buttons() && haulage) {
        const int delta(event->x() - prevPos.x());
        prevPos = event->pos();
        haulage->setPixelPosition(haulage->getPixelPosition() + delta);
        haulage->update();
        chart->update();
        emitSliderMoved(haulage);
        return;
    }

    this->setCursor(sliders.findNear(x) ? Qt::SizeHorCursor : Qt::ArrowCursor);
}

void C2dChart::mousePressEvent(QMouseEvent* event)
{
    prevPos = event->pos();
    CSlider* const s(sliders.findNear(event->x()));
    if (s && event->button() != Qt::MidButton) {
        haulage = s;
        sliders.setCurrent(haulage);
    }
}

void C2dChart::wheelEvent(QWheelEvent *e)
{
    if (e->x() > chart->geometry().left() && e->x() < chart->geometry().right()) {
        chart->acceptWheelX((e->pos() - chart->pos()).x(), e->angleDelta().y());
    }
    if (e->y() > chart->geometry().top() && e->y() < chart->geometry().bottom()) {
        chart->acceptWheelY((e->pos() - chart->pos()).y(), e->angleDelta().y());
    }
}

void C2dChart::showArray(const CArray& m)
{
    C2dChart chart;
    CChartData data;
    data.push_back(new CDimensionInterval(CGL::CInterval(0.0, static_cast<int>(1.0 * m.size() - 1.0, m.size()))));
    data.push_back(new CDimensionArray(m));
    chart.setData(data);

    QEventLoop* loop(new QEventLoop(&chart));
    loop->connect(&chart, SIGNAL(closed()), SLOT(quit()));

    chart.show();

    loop->exec();
}

void C2dChart::emitSliderMoved(CSlider* s) {
    emit sliderMoved(s);
}

void C2dChart::addSlider(CSlider* s) {
    if (sliders.contains(s)) {
        return;
    }
    s->setGeometry(chart->geometry());
    s->setRange(xAxis->getRange());
    this->scene()->addItem(s);
    sliders << s;
    if (sliders.getCurrent() == nullptr) {
        sliders.setCurrent(s);
    }
}

void C2dChart::removeSlider(CSlider* s) {
    if (!sliders.contains(s)) {
        return;
    }
    this->scene()->removeItem(s);
    sliders.removeOne(s);
    chart->repaint();
}
