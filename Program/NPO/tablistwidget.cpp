#include "tablistwidget.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QPalette>
#include <QDebug>

TabListWidget::TabListWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setLayout(new QVBoxLayout);
    layout()->setMargin(0);
    layout()->setSpacing(0);
    auto p(this->palette());
    p.setColor(QPalette::Window, Qt::white);
    this->setPalette(p);

    layout()->addStretch(20);
    stretch = layout()->takeAt(0);
}

QVBoxLayout* TabListWidget::layout()
{
    Q_ASSERT(dynamic_cast<QVBoxLayout*>(QWidget::layout()));
    return static_cast<QVBoxLayout*>(QWidget::layout());
}

TabListWidget::~TabListWidget()
{
}

void TabListWidget::setCurrentIndex(int i)
{
    QPushButton* imposter(dynamic_cast<QPushButton*>(layout()->itemAt(i << 1)->widget()));
    if (current().key() == imposter) {
        layout()->addItem(stretch);
        current().value()->setVisible(false);
    } else {
        if (current() != items.end()) {
            current().value()->hide();
        } else {
            layout()->removeItem(stretch);
        }
        items[imposter]->show();

        emit currentChanged(i);
    }
}

void TabListWidget::togleTab()
{
    Q_ASSERT(dynamic_cast<QPushButton*>(QObject::sender()));
    setCurrentIndex(toId(dynamic_cast<QWidget*>(QObject::sender())));
}

int TabListWidget::addTab(Items::iterator item)
{
    if (current() == item) {
        layout()->removeItem(stretch);
    }

    item.key()->setFlat(true);
    layout()->addWidget(item.key());
    layout()->addWidget(item.value());

    if (current() == item) {
        layout()->addItem(stretch);
    }

    item.value()->hide();
    connect(item.key(), &QPushButton::clicked, this, &TabListWidget::togleTab);
    return (layout()->count() >> 1) - 1;
}

void TabListWidget::disable(int id, bool disabled) {
    Q_ASSERT(dynamic_cast<QPushButton*>(layout()->itemAt(id * 2)->widget()));
    static_cast<QPushButton*>(layout()->itemAt(id * 2)->widget())->setDisabled(disabled);
}

TabListWidget::Items::iterator TabListWidget::current()
{
    Items::iterator it(items.begin());
    while (it != items.end() && it.value()->isHidden()) {
        ++it;
    }
    return it;
}

int TabListWidget::toId(const Items::iterator it)
{
    if (items.end() == it) {
        return -1;
    }
    int id(toId(it.key()));
    if (id < 0) {
        id = toId(it.value());
    }
    return id;
}

int TabListWidget::toId(const QWidget* const w)
{
    for (int i(0); i < layout()->count(); ++i) {
        if (layout()->itemAt(i)->widget() == w) {
            return i >> 1;
        }
    }
    return -1;
}

int TabListWidget::currentIndex()
{
    return toId(current());
}

int	TabListWidget::addTab(QWidget* page, const QString& label)
{
    return addTab(items.insert(new QPushButton(label, this), page));
}

int	TabListWidget::addTab(QWidget* page, const QIcon& icon, const QString& label)
{
    return addTab(items.insert(new QPushButton(icon, label, this), page));
}
