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
#include <QTextBrowser>
#include <QScrollArea>

#include <fem.h>
#include <c2dchart.h>
#include <cchartdata.h>
#include <cdimensioninterval.h>
#include <cdimensionarray.h>
#include <cslider.h>
#include <eigenmode.h>

#include "application.h"
#include "identity.h"
#include "femviewer.h"
#include "engine/afrarray.h"
#include "engine/afr.h"
#include "filenameedit.h"
#include "tablistwidget.h"


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
    QPushButton* const exportButton(new QPushButton(Application::identity()->tr("modes identification wizard/nethods selector/export button"), this));
    exportButton->setDisabled(true);
    buttons->addWidget(exportButton);
    buttons->addSpacing(30);
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

void ModesIdentificationWizard::accept()
{
    QDialog::accept();
}

void ModesIdentificationWizard::reject()
{
    QDialog::reject();
}

ModesIdentificationWizard::~ModesIdentificationWizard()
{
}

ModesIdentificationWizard::MethodSelector::MethodSelector(QWidget* parent)
    : TabListWidget(parent)
{
    Q_ASSERT(Application::identity()->resolveKey("modes identification wizard/methods").isArray());
    QJsonArray config(Application::identity()->resolveKey("modes identification wizard/methods").toArray());
    for (QJsonValueRef i : config) {
        QJsonObject itm(i.toObject());
        QWidget* tab(new QWidget(this));
        Q_ASSERT(itm.contains("disabled") && itm["disabled"].isBool());
        if (itm["disabled"].toBool()) {
            this->disable(this->addTab(tab, Application::identity()->tr(itm, "title")));
        } else {
            QVBoxLayout* l(new QVBoxLayout);
            tab->setLayout(l);
            l->setMargin(0);
            if (itm.contains("params") && itm["params"].isArray()) {
                ///@todo add params import
            }
            currentResults.push_back(new QTextEdit(tab));
            currentResults.last()->setReadOnly(false);
            l->addWidget(currentResults.last());
            if (QFile::exists(Application::identity()->tr(itm, "discription file"))) {
                QFile f(Application::identity()->tr(itm, "discription file"));
                if (!f.open(QFile::ReadOnly)) {
                    qFatal(QString(f.fileName() + " man file can't be open").toLocal8Bit());
                }
                QTextBrowser* doc(new QTextBrowser(tab));
                doc->setHtml(QString(f.readAll()));
                l->addWidget(doc);
            }
            this->addTab(tab, Application::identity()->tr(itm, "title"));
        }
    }

    this->setFixedWidth(335);
}

ModesIdentificationWizard::MethodSelector::~MethodSelector()
{
}

ModesIdentificationWizard::ManualController::ManualController(const FEM* const model, QWidget* parent)
    : QWidget(parent)
    , __splitter(new QSplitter(this))
    , __viewer(new FEMViewer(__splitter))
    , __chart(new C2dChart(__splitter))
    , __afr(nullptr)
    , __slider(new CSlider)
{
    QLayout* top(new QHBoxLayout);
    FileNameEdit* const input(new FileNameEdit(this));
    input->setOpenFileFunction([](QWidget* parent)->QString{
        return Application::identity()->openFileDialog("modes identification wizard/file dialog", parent);
    });
    top->addWidget(input);
    connect(input, SIGNAL(fileNameChanged(QString)), SLOT(setAFR(QString)));
    QPushButton* changeOrientation(new QPushButton(this));
    changeOrientation->setIcon(Application::identity()->icon("modes identification wizard/orientation icon"));
    changeOrientation->setToolTip(Application::identity()->tr("modes identification wizard/orientation tooltip"));
    connect(changeOrientation, SIGNAL(clicked()), SLOT(changeSplitterOrientation()));
    top->addWidget(changeOrientation);

    __splitter->setOrientation(Qt::Vertical);
    __splitter->addWidget(__viewer);
    __viewer->setModel(model);
    __chart->setGridStep(70);
    __splitter->addWidget(__chart);
    __splitter->setHandleWidth(3);
    __splitter->handle(0)->setAutoFillBackground(false);
    __splitter->setSizes(QList<int>() << 350 << 650);

    __chart->addSlider(__slider);
    connect(__chart, SIGNAL(sliderMoves(CSlider*)), SLOT(setModeFrequency(CSlider*)));
    __slider->setLabelTemplate("%1 " + Application::identity()->tr("hertz"));

    this->setLayout(new QVBoxLayout);
    this->layout()->addItem(top);
    this->layout()->addWidget(__splitter);
}

ModesIdentificationWizard::ManualController::~ManualController()
{
    delete __slider;
    ///@todo fix this tarible kluge... destructor crash the program. but it's called when i complidely close the app anyway. Only for one instance of ManualController
    __viewer->setParent(0);
}

void ModesIdentificationWizard::ManualController::setModeFrequency(CSlider* s)
{
    if (s == __slider) {
        const EigenMode& mode(__afr->getMode(s->getPosition()));
        __viewer->setProxyMode(mode);
    }
}

void ModesIdentificationWizard::ManualController::setAFR(QString filename)
{
    if (filename.split('.').last().toLower() != "unv") {
        return;
    }
    delete __afr;
    __afr = new AFRArray;
    __afr->read(filename);
    CChartDataList data(__afr->toChartData());

    if (__afr->size() > 1 && !__afr->at(1).empty()) {
        const AFR& afr(__afr->at(1));
        __slider->setPurview(CRealRange(afr.front().frequency, afr.back().frequency));
    }

    data.setChartTitle(Application::identity()->tr("title", "modes identification wizard/chart"));
    data.setDimensionTitle(Application::identity()->tr("xLabel", "modes identification wizard/chart"), 0);
    data.setDimensionTitle(Application::identity()->tr("yLabel", "modes identification wizard/chart"), 1);
    __chart->setData(data);
    __chart->update();
    setModeFrequency(__slider);

}

void ModesIdentificationWizard::ManualController::changeSplitterOrientation()
{
    __splitter->setOrientation(__splitter->orientation() == Qt::Vertical ? Qt::Horizontal : Qt::Vertical);
}

void ModesIdentificationWizard::stylize()
{
    if (QtWin::isCompositionEnabled()) {
        QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
        setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_NoSystemBackground, false);
        setStyleSheet("ModesIdentificationWizard { background: transparent; }");
    }
}

void ModesIdentificationWizard::identifyModes(const FEM* who, QWidget* parent)
{
    ModesIdentificationWizard* w(new ModesIdentificationWizard(who, parent));

    QEventLoop* loop(new QEventLoop(w));
    loop->connect(w, SIGNAL(finished(int)), SLOT(quit()));

    w->show();

    loop->exec();

    w->deleteLater();
}
