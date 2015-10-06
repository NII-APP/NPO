#include "modesidentificationwizard.h"

#include <QEventLoop>
#include <QtWinExtras>
#include <QHBoxLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QSplitter>
#include <QTableWidget>
#include <QFormLayout>
#include <QSpinBox>

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

ModesIdentificationWizard::MethodSelector::MethodSelector(QWidget* parent)
    : QWidget(parent)
    , __resultsTable(new QTableWidget(this))
{
    this->setLayout(new QVBoxLayout);
    this->layout()->addWidget(new QLabel("<h2>Идентификация параметров</h2>", this));
    Signboard* const first(new Signboard(this));
    first->setTitle("Метод выделения тона");
    Signboard* const second(new Signboard(this));
    second->setTitle("Метод полиномиальной аппроксимации");
    QWidget* const w(second->getBoard());
    QFormLayout* l(new QFormLayout);
    l->addRow(tr("Порядок модели:"), new QSpinBox(w));
    l->addRow(tr("Допуск на частоту:"), new QSpinBox(w));
    l->addRow(tr("Допуск на демпфирование:"), new QSpinBox(w));
    w->setLayout(l);
    Signboard* const third(new Signboard(this));
    third->setTitle("Метод сингулярного разложения");
    __methods << first << second << third;
    for (Signboard* i : __methods) {
        this->layout()->addWidget(i);
    }
    static_cast<QVBoxLayout*>(this->layout())->addStretch(200);
    this->layout()->addWidget(__resultsTable);

    __resultsTable->setColumnCount(3);
    __resultsTable->setRowCount(4);
    __resultsTable->setItem(0,0, new QTableWidgetItem("Номер формы"));
    __resultsTable->setItem(0,1, new QTableWidgetItem("Частота [Гц]"));
    __resultsTable->setItem(0,2, new QTableWidgetItem("Демпифрование [%]"));
    __resultsTable->setItem(1,0, new QTableWidgetItem("1"));
    __resultsTable->setItem(1,1, new QTableWidgetItem("24"));
    __resultsTable->setItem(1,2, new QTableWidgetItem("0.3"));
    __resultsTable->setItem(2,0, new QTableWidgetItem("2"));
    __resultsTable->setItem(2,1, new QTableWidgetItem("45"));
    __resultsTable->setItem(2,2, new QTableWidgetItem("0.5"));
    __resultsTable->setItem(3,0, new QTableWidgetItem("3"));
    __resultsTable->setItem(3,1, new QTableWidgetItem("220"));
    __resultsTable->setItem(3,2, new QTableWidgetItem("0.4"));
    __resultsTable->setFixedHeight(160);

    this->layout()->addWidget(new QPushButton("Экспорт форм", this));

    this->setFixedWidth(335);
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
    __splitter->setStretchFactor(__splitter->indexOf(__viewer), 300);
    __splitter->setStretchFactor(__splitter->indexOf(__chart), 200);
    __splitter->setHandleWidth(3);
    __splitter->handle(0)->setAutoFillBackground(false);


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
        absolute.setTitle(Application::identity()->tr("title", "modes identification wizard/chart"));
        xAbs->setLabel(Application::identity()->tr("xLabel", "modes identification wizard/chart"));
        yAbs->setLabel(Application::identity()->tr("yLabel", "modes identification wizard/chart"));
        data.removeFirst();
        data.removeFirst();

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

ModesIdentificationWizard::MethodSelector::Signboard::Signboard(QWidget* parent)
    : QWidget(parent)
    , __board(new QWidget(this))
    , __title(new QPushButton(this))
{
    this->setLayout(new QVBoxLayout);
    this->layout()->addWidget(__title);
    this->layout()->addWidget(__board);
}

void ModesIdentificationWizard::MethodSelector::Signboard::setTitle(const QString& t) {
    __title->setText(t);
}
QWidget* ModesIdentificationWizard::MethodSelector::Signboard::getBoard() {
    return __board;
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
