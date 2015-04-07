#include "meshscene.h"
#include "meshplace.h"
#include "../application.h"
#include <QHBoxLayout>
#include "../identity.h"

MeshScene::MeshScene(QWidget *parent)
    : QWidget(parent)
    , properties(new Properties(this))
    , place(new MeshPlace(0/*https://bugreports.qt.io/browse/QTBUG-31451*/))
{
    place->setParent(this);
    properties->move(150,150);
}

MeshScene::~MeshScene()
{

}

void MeshScene::setData(const Mesh *model)
{
    place->setData(model);
}

void MeshScene::setData(const QVector<const Mesh*>& models)
{
    place->setData(models);
}

void MeshScene::setMode(const int m)
{
    if (dynamic_cast<const MeshModes*>(place->getData().first())) {
        dynamic_cast<const MeshModes*>(place->getData().first())->colorize(m);
    }
    place->setMode(m);
}

QVector<const Mesh*> MeshScene::getData() const
{
    return place->getData();
    return QVector<const Mesh*>();
}

void MeshScene::resizeEvent(QResizeEvent *)
{
    place->setGeometry(QRect(QPoint(0,0), this->size()));
}

MeshScene::Properties::Properties(QWidget* parent)
    : QFrame(parent)
    , form(new QSpinBox(this))
    , formLabel(new QLabel(Application::identity()->formSelectorLabel(), this))
    , formSubLabel(new QLabel(form))
{
    this->setLayout(new QVBoxLayout);
    this->layout()->addWidget(form);

    this->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    this->setAutoFillBackground(true);
    QLayout* const l(new QHBoxLayout(this));
    static_cast<QVBoxLayout*>(this->layout())->addLayout(l);
    l->setMargin(5);
    this->resize(200, form->height());
    l->addWidget(formLabel);
    l->addWidget(form);
    l->addWidget(formSubLabel);
    form->setMinimum(1);
    connect(form, SIGNAL(valueChanged(int)), SLOT(setMode(int)));

}
