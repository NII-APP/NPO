#include "modesidentificationwizard.h"

#include <QEventLoop>
#ifdef Q_OS_WIN
#include <QtWinExtras>
#endif
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
#include <QJsonArray>

#include <fem.h>
#include <c2dchart.h>
#include <cchartdata.h>
#include <cdimensioninterval.h>
#include <cdimensionarray.h>
#include <cslider.h>
#include <eigenmode.h>

#include "application.h"
#include "identity.h"
#include "gui/toolbox/femviewer/femviewer.h"
#include "afr/afrarray.h"
#include "afr/afr.h"
#include "gui/toolbox/filenameedit.h"
#include "gui/toolbox/tablistwidget.h"
#include "modesidentchart.h"
#include "project.h"


ModesIdentificationWizard::ModesIdentificationWizard(const FEM* who, QWidget* parent)
    : QDialog(parent)
    , __method(new MethodSelector(this))
    , __controller(new ManualController(who, this))
    , __model(who)
    , __acceptB(new QPushButton(Application::identity()->tr("accept", "modes identification wizard"), this))
{
    QHBoxLayout* const center(new QHBoxLayout);
    center->addWidget(__method);
    center->addWidget(__controller);
    connect(__method, SIGNAL(identificationModeChanged(ModesIdentificationWizard::IdentificationMode)),
            __controller, SLOT(setIdentificationMode(ModesIdentificationWizard::IdentificationMode)));
    connect(__controller, SIGNAL(currentResultChanged(EigenModes*)),
            __method, SLOT(updateCurrentResults(EigenModes*)));
    connect(__controller, SIGNAL(currentResultChanged(EigenModes*)),
            this, SLOT(checkIsCurrentModesAcceptable()));
    __method->setCurrentMode(View);

    QHBoxLayout* const buttons(new QHBoxLayout);
    buttons->addStretch(200);
    QPushButton* const exportButton(new QPushButton(Application::identity()->tr("modes identification wizard/methods selector/export button"), this));
    exportButton->setDisabled(true);
    buttons->addWidget(exportButton);
    buttons->addSpacing(30);
    __acceptB->setDisabled(true);
    buttons->addWidget(__acceptB);
    buttons->addSpacing(30);
    connect(__acceptB, SIGNAL(clicked()), SLOT(accept()));
    QPushButton* const cancelB(new QPushButton(Application::identity()->tr("cancel", "modes identification wizard"), this));
    buttons->addWidget(cancelB);
    connect(cancelB, SIGNAL(clicked()), SLOT(reject()));

    this->setLayout(new QVBoxLayout(this));
    this->layout()->addItem(center);
    this->layout()->addItem(buttons);

    stylize();

    this->resize(1300,800);
}

void ModesIdentificationWizard::checkIsCurrentModesAcceptable()
{
    __acceptB->setEnabled(__controller->currentResult() == nullptr ? false : true);
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
            QTextBrowser* resultDisplay(nullptr);
            tab->setLayout(l);
            l->setMargin(0);
            if (itm.contains("params") && itm["params"].isArray()) {
                ///@todo add params import
                resultDisplay = new QTextBrowser(tab);
                resultDisplay->setReadOnly(false);
                l->addWidget(resultDisplay);
            }
            if (QFile::exists(Application::identity()->tr(itm, "discription file"))) {
                QFile f(Application::identity()->tr(itm, "discription file"));
                if (!f.open(QFile::ReadOnly)) {
                    qFatal(QString(f.fileName() + " man file can't be open").toLocal8Bit());
                }
                QTextBrowser* doc(new QTextBrowser(tab));
                doc->setHtml(QString(f.readAll()));
                l->addWidget(doc);
            }
            __resultDisplays.insert(this->addTab(tab, Application::identity()->tr(itm, "title")), resultDisplay);
        }
    }
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(changeCurrentIdentMode(int)));

    this->setFixedWidth(335);
}

ModesIdentificationWizard::MethodSelector::~MethodSelector()
{
}

EigenModes* ModesIdentificationWizard::currentResult() const
{
    return __controller->currentResult();
}

void ModesIdentificationWizard::MethodSelector::setCurrentMode(ModesIdentificationWizard::IdentificationMode t)
{
    switch (t) {
    case View:
        this->setCurrentIndex(0);
        return;
    case Pick:
        this->setCurrentIndex(1);
        return;
    }
}


void ModesIdentificationWizard::MethodSelector::changeCurrentIdentMode(int i)
{
    switch (i) {
    case 0:
        emit identificationModeChanged(View);
        return;
    case 1:
        emit identificationModeChanged(Pick);
        return;
    }
}

void ModesIdentificationWizard::MethodSelector::updateCurrentResults(EigenModes* r)
{
    //r is null if signal emited just to disable acceptButton
    if (r == nullptr) {
        return;
    }
    QString result;
    static const QString stencil(Application::identity()->tr("modes identification wizard/methods selector/result template"));
    for (int i(0); i != r->size(); ++i) {
        result += stencil.arg(QString::number(i + 1),
                              QString::number(r->at(i).frequency()),
                              QString::number(r->at(i).averageDamping())) + "<br/>";
    }
    __resultDisplays[this->currentIndex()]->setHtml(result);
}

ModesIdentificationWizard::ManualController::ManualController(const FEM* const model, QWidget* parent)
    : QWidget(parent)
    , __splitter(new QSplitter(this))
    , __viewer(new FEMViewer(__splitter))
    , __chart(new ModesIdentChart(__splitter))
    , __afr(nullptr)
{
    QLayout* top(new QHBoxLayout);
    FileNameEdit* const input(new FileNameEdit(this));
    input->setOpenFileFunction([](FileNameEdit* w)->QString{
        return Application::identity()->openFileDialog("modes identification wizard/file dialog", w);
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
    __splitter->addWidget(__chart);
    __splitter->setHandleWidth(3);
    __splitter->handle(0)->setAutoFillBackground(false);
    __splitter->setSizes(QList<int>() << 350 << 650);

    connect(__chart, SIGNAL(newCurrentFrequency(double)), this, SLOT(setModeFrequency(double)));
    connect(__chart, SIGNAL(currentResultChanged(EigenModes*)), this, SLOT(postResultChanges(EigenModes*)));

    this->setLayout(new QVBoxLayout);
    this->layout()->addItem(top);
    this->layout()->addWidget(__splitter);
}

void ModesIdentificationWizard::ManualController::postResultChanges(EigenModes* m)
{
    emit currentResultChanged(m);
}

ModesIdentificationWizard::ManualController::~ManualController()
{
    ///@todo fix this tarible kluge... destructor crash the program. but it's called when i complidely close the app anyway. Only for one instance of ManualController
    __viewer->setParent(0);
    __chart->setParent(0);
}

void ModesIdentificationWizard::ManualController::setModeFrequency(double v)
{
    __viewer->setProxyMode(__afr->getMode(v));
}

void ModesIdentificationWizard::ManualController::setAFR(QString filename)
{
    if (filename.split('.').last().toLower() != "unv") {
        return;
    }
    delete __afr;
    __afr = new AFRArray;
    __afr->read(filename);
    if (__afr->size() == 0) {
        QMessageBox::warning(static_cast<QWidget*>(this->parent()),
                             Application::identity()->tr("modes identification wizard/file warning/title"),
                             Application::identity()->tr("modes identification wizard/file warning/text")
                                .arg(filename));
        return;
    }
    if (__afr->size() - 1 != __viewer->getModel()->getNodes().length()) {
        QMessageBox::warning(static_cast<QWidget*>(this->parent()),
                             Application::identity()->tr("modes identification wizard/size warning/title"),
                             Application::identity()->tr("modes identification wizard/size warning/text")
                                .arg(QString::number(__viewer->getModel()->getNodes().length()),
                                     QString::number(__afr->size() - 1),
                                     filename));
    }

    __chart->setData(*__afr);

}

void ModesIdentificationWizard::ManualController::changeSplitterOrientation()
{
    __splitter->setOrientation(__splitter->orientation() == Qt::Vertical ? Qt::Horizontal : Qt::Vertical);
}

void ModesIdentificationWizard::ManualController::setIdentificationMode(ModesIdentificationWizard::IdentificationMode mode)
{
    __chart->setIdentMode(mode);
}

void ModesIdentificationWizard::stylize()
{
#ifdef Q_OS_WIN
    if (QtWin::isCompositionEnabled()) {
        QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
        setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_NoSystemBackground, false);
        setStyleSheet("ModesIdentificationWizard { background: transparent; }");
    }
#endif
}

EigenModes* ModesIdentificationWizard::ManualController::currentResult() {
    return __chart->currentResult();
}
