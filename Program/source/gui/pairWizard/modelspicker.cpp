#include "modelspicker.h"
#include "project.h"
#include "application.h"
#include <QComboBox>
#include <QVBoxLayout>
#include <cassert>
#include "fem.h"

ModelsPicker::ModelsPicker(QWidget *parent)
    : QWidget(parent)
    , first(new QComboBox(this))
    , second(new QComboBox(this))
{
    this->setLayout(new QVBoxLayout(this));
    this->layout()->addWidget(first);
    this->layout()->addWidget(second);

    auto update = [this](QComboBox* const w2, QComboBox* const w1){
        w1->blockSignals(true);
        int cur(w1->currentData().toInt());
        while (w1->count()) {
            w1->removeItem(0);
        }
        int j(1);
        for (const FEM* const i : femList()) {
            if (j != w2->currentData().toInt()) {
                w1->insertItem(w1->count(), i->getName(), QVariant(j));
            }
            ++j;
        }
        w1->setCurrentIndex(cur - 1 - (cur > w2->currentData().toInt()));
        w1->blockSignals(false);
#ifndef QT_NO_DEBUG
        qDebug() << "picked new modesPair" << (a() ? a()->getName() : "null") << (b() ? b()->getName() : "null");
#endif
        emit modelsPicked(std::pair<const FEM*, const FEM*>(a(), b()));
    };
    update(first, second);
    update(second, first);
    void (QComboBox:: *indexChangedSignal)(int) = &QComboBox::currentIndexChanged;
    connect(first,  indexChangedSignal, [this, update]() { update(first, second); });
    connect(second, indexChangedSignal, [this, update]() { update(second, first); });
}

Project::ConstModels ModelsPicker::femList()
{
    Project::ConstModels l(Application::project()->FEMList());
    Project::ConstModels::const_iterator it(l.begin());
    while (it != l.end()) {
        if ((*it)->getModes().empty()) {
            it = l.erase(it);
        } else {
            ++it;
        }
    }
    return l;
}

const FEM* ModelsPicker::a()
{
    if (first->currentData().toInt()) {
        return femList().at(first->currentData().toInt() - 1);
    } else {
        return nullptr;
    }
}

const FEM* ModelsPicker::b()
{
    if (second->currentData().toInt()) {
        return femList().at(second->currentData().toInt() - 1);
    } else {
        return nullptr;
    }
}

void ModelsPicker::setPick(const std::pair<const FEM*, const FEM*>& p)
{
    int i(0);
    bool k(0);
    const Project::ConstModels l(femList());
    while (i < l.size()) {
        if (l[i] == p.first) {
            first->setCurrentIndex(i - k);
            ++k;
        }
        if (l[i] == p.second) {
            second->setCurrentIndex(i - k);
            ++k;
        }
        ++i;
    }
}

ModelsPicker::~ModelsPicker()
{

}

