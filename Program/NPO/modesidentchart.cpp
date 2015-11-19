#include "modesidentchart.h"

#include <QToolBar>
#include <QMouseEvent>

#include <c2dchart.h>
#include <cslider.h>

#include "application.h"
#include "identity.h"
#include "engine/afr.h"
#include "engine/afrarray.h"

ModesIdentChart::ModesIdentChart(QWidget *parent)
    : QWidget(parent)
    , __chart(new C2dChart(this))
    , __toolbar(new QToolBar(this))
    , __re(new QAction(Application::identity()->icon(":/media/images/re.png"), Application::identity()->tr("modes identification wizard/chart/re"), __toolbar))
    , __im(new QAction(Application::identity()->icon(":/media/images/im.png"), Application::identity()->tr("modes identification wizard/chart/im"), __toolbar))
    , __am(new QAction(Application::identity()->icon(":/media/images/amp.png"), Application::identity()->tr("modes identification wizard/chart/amplitude"), __toolbar))
    , __average(new QAction(Application::identity()->icon(":/media/images/average.png"), Application::identity()->tr("modes identification wizard/chart/average"), __toolbar))
{
    qApp->installEventFilter(this);
    __toolbar->setAutoFillBackground(true);
    __toolbar->hide();
    QPalette p(__toolbar->palette());
    p.setColor(QPalette::Window, Qt::white);
    __toolbar->setPalette(p);

    __chart->setGridStep(70);
    __chart->move(0,0);
    //__slider->setLabelTemplate("%1 " + Application::identity()->tr("hertz"));
    __average->setCheckable(true);
    __average->setChecked(true);
    __toolbar->addAction(__average);

    __toolbar->addSeparator();

    __re->setCheckable(true);
    __re->setChecked(true);
    __toolbar->addAction(__re);
    __im->setCheckable(true);
    __im->setChecked(true);
    __toolbar->addAction(__im);
    __am->setCheckable(true);
    __am->setChecked(true);
    __toolbar->addAction(__am);

    connect(__re, SIGNAL(triggered()), this, SLOT(update()));
    connect(__im, SIGNAL(triggered()), this, SLOT(update()));
    connect(__am, SIGNAL(triggered()), this, SLOT(update()));
    connect(__average, SIGNAL(triggered()), this, SLOT(update()));
}

void ModesIdentChart::leaveEvent(QEvent *)
{
    __toolbar->setVisible(false);
    __chart->move(0,0);
    __chart->resize(this->size());
}
void ModesIdentChart::enterEvent(QEvent *)
{
    __toolbar->setVisible(true);
    __chart->move(0, __toolbar->height());
    __chart->resize(this->size() -= QSize(0, __toolbar->height()));
}

void ModesIdentChart::resizeEvent(QResizeEvent *)
{
    __toolbar->resize(this->size().width(), __toolbar->height());
    __chart->move(0, __toolbar->height() * __toolbar->isVisible());
    __chart->resize(this->size() -= QSize(0, __toolbar->height() * __toolbar->isVisible()));
}

void ModesIdentChart::setData(const AFRArray &afrArray)
{
    __afr.amplitude = afrArray.toChartData(AFR::Amplitude);
    __afr.imaginary = afrArray.toChartData(AFR::Imaginary);
    __afr.real = afrArray.toChartData(AFR::Real);
    const AFR average = afrArray.average();
    __averageAfr.amplitude = average.toChartData(AFR::Amplitude);
    __averageAfr.imaginary = average.toChartData(AFR::Imaginary);
    __averageAfr.real = average.toChartData(AFR::Real);

    if (afrArray.size() > 1) {
        int i(0);
        while (afrArray.at(i).empty() && i <  afrArray.size()) {
            ++i;
        }
        Q_ASSERT(i != afrArray.size());
        if (i != afrArray.size()) {
            const AFR& afr(afrArray.at(i));
            for (auto s : __sliders) {
                s->setPurview(CRealRange(afr.front().frequency, afr.back().frequency));
            }
        }
    }
    update();
}

void ModesIdentChart::update()
{
    CChartDataList data;
    ChartData& source(__average->isChecked() ? __averageAfr : __afr);
    if (__re->isChecked()) {
        data << source.real;
    }
    if (__im->isChecked()) {
        data << source.imaginary;
    }
    if (__am->isChecked()) {
        data << source.amplitude;
    }
    data.setChartTitle(Application::identity()->tr("title", "modes identification wizard/chart"));
    data.setDimensionTitle(Application::identity()->tr("xLabel", "modes identification wizard/chart"), 0);
    data.setDimensionTitle(Application::identity()->tr("yLabel", "modes identification wizard/chart"), 1);

    __chart->setData(data);
    __chart->update();
}
