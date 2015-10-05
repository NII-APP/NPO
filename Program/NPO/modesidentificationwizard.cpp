#include "modesidentificationwizard.h"

#include <QEventLoop>
#include <QtWinExtras>
#include <QHBoxLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QSplitter>

#include <fem.h>
#include <c2dchart.h>
#include <cchartdata.h>
#include <cdimensioninterval.h>
#include <cdimensionarray.h>

#include "application.h"
#include "identity.h"
#include "femviewer.h"
#include "engine/afrarray.h"
#include "engine/afr.h"

ModesIdentificationWizard::ModesIdentificationWizard(const FEM* who, QWidget* parent)
    : QDialog(parent)
    , __method(new MethodSelector(this))
    , __controller(new ManualController(who, this))
    , __model(who)
{
    QHBoxLayout* const center(new QHBoxLayout);
    center->addWidget(__method);
    center->addWidget(__controller);

    QHBoxLayout* const buttons(new QHBoxLayout);
    buttons->addStretch(200);
    QPushButton* const acceptB(new QPushButton(Application::identity()->tr("accept", "modes identification wizard"), this));
    buttons->addWidget(acceptB);
    buttons->addSpacing(30);
    connect(acceptB, SIGNAL(clicked()), SLOT(accept()));
    QPushButton* const cancelB(new QPushButton(Application::identity()->tr("cancel", "modes identification wizard"), this));
    buttons->addWidget(cancelB);
    connect(cancelB, SIGNAL(clicked()), SLOT(reject()));

    this->setLayout(new QVBoxLayout(this));
    this->layout()->addItem(center);
    this->layout()->addItem(buttons);

    stylize();

    this->resize(1300,800);
}

void ModesIdentificationWizard::accept() {
    QDialog::accept();
}

void ModesIdentificationWizard::reject() {
    QDialog::reject();
}
ModesIdentificationWizard::~ModesIdentificationWizard()
{

}

ModesIdentificationWizard::ManualController::ManualController(const FEM* const model, QWidget* parent)
    : QWidget(parent)
    , __splitter(new QSplitter(this))
    , __viewer(new FEMViewer(this))
    , __chart(new C2dChart(this))
    , __afr(nullptr)
{
    QLayout* top(new QHBoxLayout);
    FileInput* const input(new FileInput(this));
    top->addWidget(input);
    connect(input, SIGNAL(fileNameChanged(QString)), SLOT(setAFR(QString)));
    QPushButton* changeOrientation(new QPushButton(this));
    changeOrientation->setIcon(Application::identity()->icon("modes identification wizard/orientation icon"));
    changeOrientation->setToolTip(Application::identity()->tr("modes identification wizard/orientation tooltip"));
    connect(changeOrientation, SIGNAL(clicked()), SLOT(changeSplitterOrientation()));
    top->addWidget(changeOrientation);

    __splitter->addWidget(__viewer);
    __viewer->setModel(model);
    __splitter->addWidget(__chart);
    __splitter->setStretchFactor(0, 3);
    __splitter->setStretchFactor(1, 10);


    this->setLayout(new QVBoxLayout);
    this->layout()->addItem(top);
    this->layout()->addWidget(__splitter);
}

void ModesIdentificationWizard::ManualController::setAFR(QString filename) {
    delete __afr;
    __afr = new AFRArray;
    __afr->read(filename);

    CChartDataList data;
    if (false) {
        for (int i(0); i != __afr->size(); ++i) {
            const AFR& single(__afr->at(i));
            if (!single.empty()) {
                data.push_back(CChartData());
                CChartData& top(data.last());
                CDimensionArray* x(new CDimensionArray(static_cast<int>(single.size())));
                CDimensionArray* y(new CDimensionArray(static_cast<int>(single.size())));
                double* xIt(x->data());
                double* yIt(y->data());
                for (AFR::const_iterator it(single.begin()); it != single.end(); ++it, ++xIt, ++yIt) {
                    *xIt = it->frequency;
                    *yIt = abs(it->amplitude);
                }
                top.push_back(x);
                top.push_back(y);
                x->updateRange();
                y->updateRange();
            }
        }
    } else {
        const AFR single(__afr->average());
        data.push_back(CChartData());
        CChartData& real(data.last());
        data.push_back(CChartData());
        CChartData& im(data.last());
        data.push_back(CChartData());
        CChartData& absolute(data.last());
        CDimensionArray* xRe(new CDimensionArray(static_cast<int>(single.size())));
        CDimensionArray* yRe(new CDimensionArray(static_cast<int>(single.size())));
        CDimensionArray* xIm(new CDimensionArray(static_cast<int>(single.size())));
        CDimensionArray* yIm(new CDimensionArray(static_cast<int>(single.size())));
        CDimensionArray* xAbs(new CDimensionArray(static_cast<int>(single.size())));
        CDimensionArray* yAbs(new CDimensionArray(static_cast<int>(single.size())));
        double* xReIt(xRe->data());
        double* yReIt(yRe->data());
        double* xImIt(xIm->data());
        double* yImIt(yIm->data());
        double* xAbsIt(xAbs->data());
        double* yAbsIt(yAbs->data());
        for (AFR::const_iterator it(single.begin()); it != single.end(); ++it, ++xReIt, ++yReIt, ++xImIt, ++yImIt, ++xAbsIt, ++yAbsIt) {
            *xReIt = *xImIt = *xAbsIt = it->frequency;
            *yAbsIt = abs(it->amplitude);
            *yReIt = it->amplitude.real();
            *yImIt = it->amplitude.imag();
        }
        real.push_back(xRe);
        real.push_back(yRe);
        im.push_back(xIm);
        im.push_back(yIm);
        absolute.push_back(xAbs);
        absolute.push_back(yAbs);
        xRe->updateRange();
        yRe->updateRange();
        xIm->updateRange();
        yIm->updateRange();
        xAbs->updateRange();
        yAbs->updateRange();
        absolute.setTitle("Йа Графег!");
        xAbs->setLabel("Йа частота!");
        yAbs->setLabel("Йа АМплеТуда!");

    }
    __chart->setData(data);
}

void ModesIdentificationWizard::ManualController::changeSplitterOrientation() {
    __splitter->setOrientation(__splitter->orientation() == Qt::Vertical ? Qt::Horizontal : Qt::Vertical);
}

void ModesIdentificationWizard::stylize() {
    if (QtWin::isCompositionEnabled()) {
        QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
        setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_NoSystemBackground, false);
        setStyleSheet("ModesIdentificationWizard { background: transparent; }");
    }
}

void ModesIdentificationWizard::identifyModes(const FEM* who, QWidget* parent) {
    ModesIdentificationWizard* w(new ModesIdentificationWizard(who, parent));

    QEventLoop* loop(new QEventLoop(w));
    loop->connect(w, SIGNAL(finished(int)), SLOT(quit()));

    w->show();

    loop->exec();

    delete w;
}

void ModesIdentificationWizard::ManualController::FileInput::keyPressEvent(QKeyEvent * e) {
    QLineEdit::keyPressEvent(e);
    if (this->text().split('.').last().toLower() == "unv" && QFile::exists(this->text())) {
        emit fileNameChanged(this->text());
    }
}
void ModesIdentificationWizard::ManualController::FileInput::resizeEvent(QResizeEvent * e) {
    if (e) {
        QLineEdit::resizeEvent(e);
    }
    this->__pb->move(QPoint(this->width() - 25, -5));
}

ModesIdentificationWizard::ManualController::FileInput::FileInput(QWidget* parent)
    : QLineEdit(parent)
    , __pb(new QPushButton(this))
{
    __pb->setIcon(Application::identity()->icon("modes identification wizard/open file icon"));
    __pb->setToolTip(Application::identity()->tr("open file tooltip", "modes identification wizard"));
    __pb->setFlat(true);
    __pb->setFixedHeight(this->height());
    connect(__pb, SIGNAL(clicked()), SLOT(callDialod()));
    resizeEvent(0);
}

void ModesIdentificationWizard::ManualController::FileInput::callDialod() {
    const QString s(Application::identity()->openFileDialog("modes identification wizard/file dialog"));
    if (s.split('.').last().toLower() != "unv" || !QFile::exists(s)) {
        return;
    }
    this->setText(s);
    emit fileNameChanged(this->text());
}
