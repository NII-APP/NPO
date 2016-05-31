#include "relationmodeinput.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDebug>

#include <cassert>

#include "application.h"
#include "identity.h"

class RelationModeInput::SpinBox : public QSpinBox
{
    const CIndexes& relations() const { return static_cast<RelationModeInput*>(this->parent())->relations(); }
    SpinBox* left()  const { return static_cast<RelationModeInput*>(this->parent())->left; }
    SpinBox* right() const { return static_cast<RelationModeInput*>(this->parent())->right; }
public:
    SpinBox(QWidget *parent) : QSpinBox(parent) { assert(dynamic_cast<RelationModeInput*>(parent) != nullptr); this->setMinimum(1); }
    void stepBy(int d)
    {
        const CIndexes& r(relations());

        int cur(left()->value() - 1);
        CIndexes::const_iterator i(r.begin() + cur);
        assert(r.end() > i);
        if (d > 0) {
            while (i < r.end() && d) {
                ++i;
                if (i != r.end() && *i >= 0) {
                    --d;
                }
            }
            if (i == r.end()) {
                do {
                    --i;
                } while (i != r.begin() && *i < 0);
            }
        } else if (d < 0) {
            while (i > r.begin() && d) {
                --i;
                if (i >= r.begin() && *i >= 0) {
                    ++d;
                }
            }
            if (d) {
                return;
            }
            if (i < r.begin()) {
                do {
                    ++i;
                } while (i < r.end() - 1 && *i < 0);
            }
        }
        left()->setValue(i - r.begin() + 1);
        if (*i >= 0) {
            right()->setValue(*i + 1);
        }
        static_cast<RelationModeInput*>(this->parent())->emitValues();
    }
};

void RelationModeInput::emitValues()
{
    emit valueChanged(left->value() - 1, right->value() - 1);
}

RelationModeInput::RelationModeInput(QWidget *parent)
    : QFrame(parent)
    , left(new SpinBox(this))
    , right(new SpinBox(this))
{
    this->setLayout(new QHBoxLayout(this));
    this->layout()->addWidget(new QLabel(Application::identity()->tr("mode", "FEMViewer"), this));
    this->layout()->addWidget(left);
    this->layout()->addWidget(right);
    connect(left, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &RelationModeInput::emitValues);
    connect(right, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &RelationModeInput::emitValues);
}

RelationModeInput::~RelationModeInput() { }

void RelationModeInput::setMaximum(int a, int b)
{
    left->setMaximum(a);
    right->setMaximum(b);
}

void RelationModeInput::updateRelations(const CIndexes& r)
{
    relation = r;
    left->stepBy(0);
}

void RelationModeInput::updateRelations()
{
    left->stepBy(0);
}

const CIndexes& RelationModeInput::relations()
{
    return relation;
}
