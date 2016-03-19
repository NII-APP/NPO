#include "modesidentchart.h"

#include <QToolBar>
#include <QMouseEvent>
#include <QLabel>
#include <QPushButton>
#include <QLayout>

#include <c2dchart.h>
#include <cslider.h>
#include <eigenmodes.h>

#include "application.h"
#include "identity.h"
#include "afr/afr.h"
#include "afr/afrarray.h"

ModesIdentChart::ModesIdentChart(QWidget *parent)
    : QWidget(parent)
    , __chart(new C2dChart(this))
    , __toolbar(new QToolBar(this))
    , __currentMode(ModesIdentificationWizard::View)
    , __re(new QAction(Application::identity()->icon(":/media/resource/images/re.png"), Application::identity()->tr("modes identification wizard/chart/re"), __toolbar))
    , __im(new QAction(Application::identity()->icon(":/media/resource/images/im.png"), Application::identity()->tr("modes identification wizard/chart/im"), __toolbar))
    , __am(new QAction(Application::identity()->icon(":/media/resource/images/amp.png"), Application::identity()->tr("modes identification wizard/chart/amplitude"), __toolbar))
    , __average(new QAction(Application::identity()->icon(":/media/resource/images/average.png"), Application::identity()->tr("modes identification wizard/chart/average"), __toolbar))
    , __pickFreq(new QAction(QIcon(":/docs/resource/identificationMethodsDiscription/img/ok.png"), "", __toolbar))
    , __data(nullptr)
{
    qApp->installEventFilter(this);
    __toolbar->setAutoFillBackground(true);
    __toolbar->hide();
    QPalette p(__toolbar->palette());
    p.setColor(QPalette::Window, Qt::white);
    __toolbar->setPalette(p);

    __chart->setGridStep(70);
    __chart->move(0,0);
    connect(__chart, SIGNAL(sliderMoved(CSlider*)), SLOT(slidersPotrol(CSlider*)));
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
    connect(__pickFreq, SIGNAL(triggered()), this, SLOT(pickMode()));
    __toolbar->addAction(__pickFreq);

    connect(__re, SIGNAL(triggered()), this, SLOT(update()));
    connect(__im, SIGNAL(triggered()), this, SLOT(update()));
    connect(__am, SIGNAL(triggered()), this, SLOT(update()));
    connect(__average, SIGNAL(triggered()), this, SLOT(update()));
}

ModesIdentChart::~ModesIdentChart()
{
    qDeleteAll(__sliders.keys());
    qDeleteAll(__currentResults);
    for (QList<CSlider*>& i : __resultSliders) {
        qDeleteAll(i);
    }
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
            __purview = CRealRange(afr.front().frequency, afr.back().frequency);
            for (auto s : __sliders.keys()) {
                s->setPurview(__purview);
            }
        }
    }
    update();
    __data = &afrArray;
    setIdentMode(__currentMode);
}

ModesIdentChart::SliderRole ModesIdentChart::valueSliderRole(ModesIdentificationWizard::IdentificationMode m)
{
    switch (m) {
    case ModesIdentificationWizard::Pick:
        return Pick;
    default:
        return WrongRole;
    }
}

void ModesIdentChart::pickMode()
{
    Q_ASSERT(ModesIdentificationWizard::Pick == __currentMode);
    CSlider* const s(new CSlider);
    if (!__resultSliders.contains(__currentMode)) {
        __resultSliders.insert(__currentMode, QList<CSlider*>());
    }
    if (!__currentResults.contains(__currentMode) || __currentResults[__currentMode] == nullptr) {
        __currentResults[__currentMode] = new EigenModes;
    }
    s->setLabelTemplate("%1 " + Application::identity()->tr("hertz"));
    s->setDragable(false);
    __resultSliders[__currentMode].append(s);
    __chart->addSlider(s);
    s->setPosition(__sliders.key(valueSliderRole(__currentMode))->getPosition());
    if (__currentMode == ModesIdentificationWizard::Pick) {
        const EigenMode mod(__data->getMode(s->getPosition()));
        qDebug() << "alpha";
        __currentResults[__currentMode]->push_back(mod);
        qDebug() << "sort";
        std::sort(__currentResults[__currentMode]->begin(), __currentResults[__currentMode]->end(),
                  [](const EigenMode& a, const EigenMode& b)->bool{ return a.frequency() < b.frequency(); });
        qDebug() << "end";
    }
    currentResultChanged(__currentResults[__currentMode]);
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

void ModesIdentChart::slidersPotrol(CSlider* s) {
    Q_ASSERT(__sliders.contains(s));
    switch (__sliders[s]) {
    case View: case Pick:
        if (__data != nullptr) {
            emit newCurrentFrequency(s->getPosition());
        }
        return;
    case PickBound: {
        __sliders.key(Pick)->setPurview(pickRange());
        double curFreq(__data->average().findEigenFreq(pickRange()).frequency);
        __sliders.key(Pick)->setPosition(curFreq);
        if (__data != nullptr) {
            emit newCurrentFrequency(__sliders.key(Pick)->getPosition());
        }}
        return;
    case WrongRole:
        qFatal("wrong slider role");
        return;
    }
}

CRealRange ModesIdentChart::pickRange() {
    CSlider* b1(nullptr);
    CSlider* b2(nullptr);
    for (CSlider* s: __sliders.keys()) {
        if (__sliders[s] == PickBound) {
            Q_ASSERT(b2 == nullptr);
            (b1 == nullptr ? b1 : b2) = s;
        }
    }
    Q_ASSERT(b2 != nullptr && b1 != nullptr);
    return CRealRange(std::min(b1->getPosition(), b2->getPosition()),
                     std::max(b1->getPosition(), b2->getPosition()));
}

void ModesIdentChart::enableSliders(int v)
{
    typedef QMap<CSlider*, SliderRole> Sliders;
    for (Sliders::iterator i(__sliders.begin()); i != __sliders.end(); ++i) {
        if (i.value() & v) {
            i.key()->show();
            __chart->addSlider(i.key());
        } else {
            i.key()->hide();
            __chart->removeSlider(i.key());
        }
    }
}

void ModesIdentChart::setIdentMode(ModesIdentificationWizard::IdentificationMode t)
{
    if (__data == nullptr) {
        return;
    }
    switch (__currentMode = t) {
    case ModesIdentificationWizard::View:
        __pickFreq->setEnabled(false);
        if (!__sliders.values().contains(View)) {
            __sliders.insert(new CSlider, View);
            __chart->addSlider(__sliders.key(View));
            __sliders.key(View)->setLabelTemplate("%1 " + Application::identity()->tr("hertz"));
            __sliders.key(View)->setPurview(__purview);
            slidersPotrol(__sliders.key(View));
        }
        enableSliders(View);
        emit currentResultChanged(nullptr);
        return;
    case ModesIdentificationWizard::Pick:
        __pickFreq->setEnabled(true);
        if (!__sliders.values().contains(PickBound)) {
            CSlider* b1;
            CSlider* b2;
            __sliders.insert(b1 = new CSlider, PickBound);
            __sliders.insert(b2 = new CSlider, PickBound);
            b1->setLabelTemplate("%1 " + Application::identity()->tr("hertz"));
            b1->setPurview(__purview);
            b2->setLabelTemplate("%1 " + Application::identity()->tr("hertz"));
            b2->setPurview(__purview);
            b1->setPosition(__purview.getMin());
            b2->setPosition(__purview.getMax());
        }
        if (!__sliders.values().contains(Pick)) {
            CSlider* s;
            __sliders.insert(s = new CSlider, Pick);
            s->setPurview(pickRange());
            s->setLabelTemplate("%1 " + Application::identity()->tr("hertz"));
        }
        slidersPotrol(__sliders.key(PickBound));
        enableSliders(Pick | PickBound);
        if (!__currentResults.contains(ModesIdentificationWizard::Pick)) {
            __currentResults.insert(ModesIdentificationWizard::Pick, nullptr);
        }
        emit currentResultChanged(__currentResults[ModesIdentificationWizard::Pick]);
        return;
    }
    updateResultsSliders();
}

void ModesIdentChart::updateResultsSliders()
{
    typedef QMap<ModesIdentificationWizard::IdentificationMode, QList<CSlider*>> ResultsMap;

    for (ResultsMap::iterator i(__resultSliders.begin()); i != __resultSliders.end(); ++i) {
        if (i.key() == __currentMode) {
            for (CSlider* s : i.value()) {
                __chart->removeSlider(s);
            }
        } else {
            for (CSlider* s : i.value()) {
                __chart->addSlider(s);
            }
        }
    }
}

EigenModes* ModesIdentChart::currentResult()
{
    if (__currentResults.contains(__currentMode)) {
        return __currentResults[__currentMode];
    } else {
        return nullptr;
    }
}
