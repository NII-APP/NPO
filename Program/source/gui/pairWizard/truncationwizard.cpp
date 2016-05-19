#include "truncationwizard.h"
#include <QEventLoop>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QEventLoop>
#include <QSplitter>
#include <QCheckBox>

#include "fempair.h"
#include "application.h"
#include "project.h"
#include "gui/toolbox/femviewer/fempairviewer.h"
#include "modelspicker.h"
#include "relationdialog.h"

TruncationWizard::TruncationWizard(QWidget *parent)
    : QDialog(parent)
    , acceptB(new QPushButton(Application::identity()->tr("accept", "pair wizard"), this))
    , result(nullptr)
    , base(new QSplitter(this))
    , leftSide(new QWidget(base))
    , align(new QCheckBox(Application::identity()->tr("auto align", "pair wizard"), leftSide))
    , scale(new QCheckBox(Application::identity()->tr("auto scale", "pair wizard"), leftSide))
    , relations(new RelationDialog(leftSide))
    , picker(new ModelsPicker(leftSide))
    , viewer(new FEMPairViewer(base))
{
    QHBoxLayout* const buttons(new QHBoxLayout);
    buttons->addStretch(200);
    acceptB->setDisabled(true);
    buttons->addWidget(acceptB);
    buttons->addSpacing(30);
    connect(acceptB, SIGNAL(clicked()), SLOT(accept()));
    QPushButton* const cancelB(new QPushButton(Application::identity()->tr("cancel", "pair wizard"), this));
    buttons->addWidget(cancelB);
    connect(cancelB, SIGNAL(clicked()), SLOT(reject()));

    leftSide->setLayout(new QVBoxLayout(leftSide));
    leftSide->layout()->addWidget(picker);
    align->setChecked(true);
    leftSide->layout()->addWidget(align);
    scale->setChecked(true);
    leftSide->layout()->addWidget(scale);
    static_cast<QVBoxLayout*>(leftSide->layout())->addWidget(relations, 200);
    QPushButton* const resetRelations(new QPushButton(Application::identity()->tr("reset relations", "pair wizard"), leftSide));
    connect(resetRelations, &QPushButton::clicked, [this]() {
        result->calculateRelations();
        relations->scarfUpRelations();
    });
    leftSide->layout()->addWidget(resetRelations);
    base->addWidget(leftSide);
    base->addWidget(viewer);
    base->setSizes(QList<int>() << 300 << 1000);
    this->resize(1300,800);

    this->setLayout(new QVBoxLayout(this));
    this->layout()->addWidget(base);
    this->layout()->addItem(buttons);

    connect(align, &QCheckBox::clicked, this, &TruncationWizard::update);
    connect(scale, &QCheckBox::clicked, this, &TruncationWizard::update);
    connect(picker, &ModelsPicker::modelsPicked, this, &TruncationWizard::update);

    picker->setPick(std::pair<const FEM*, const FEM*>(Application::project()->FEMList().at(2),
                                                      Application::project()->FEMList().at(3)));
}

TruncationWizard::~TruncationWizard() {
}

void TruncationWizard::update()
{
    delete result;
    if (picker->a() && picker->b()) {
        result = new FEMPair(picker->a(), picker->b(), align->isChecked(), scale->isChecked());
    } else {
        result = nullptr;
    }
    relations->setPair(result);
    viewer->setPair(result);
}

FEMPair* TruncationWizard::exec(QWidget* parent)
{
    TruncationWizard w(parent);
    QEventLoop* loop(new QEventLoop(&w));
    connect(&w, &TruncationWizard::finished, loop, &QEventLoop::quit);
    w.show();
    loop->exec();
    return w.result;
}
