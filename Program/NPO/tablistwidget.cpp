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

int TabListWidget::addTab(Items::iterator item)
{
    if (current() == items.end()) {
        layout()->removeItem(stretch);
    }

    item.key()->setFlat(true);
    layout()->addWidget(item.key());
    layout()->addWidget(item.value());

    if (current() == items.end()) {
        layout()->addItem(stretch);
    }

    item.value()->hide();
    connect(item.key(), &QPushButton::clicked, this, &TabListWidget::togleTab);
    return layout()->count() >> 1 - 1;
}

void TabListWidget::togleTab()
{
    Q_ASSERT(dynamic_cast<QPushButton*>(QObject::sender()));
    if (current().key() == QObject::sender()) {
        if (current().value()->isHidden()) {
            layout()->removeItem(stretch);
            current().value()->setVisible(true);
        } else {
            layout()->addItem(stretch);
            current().value()->setVisible(false);
        }
    } else {
        if (current() != items.end()) {
            current().value()->hide();
        } else {
            layout()->removeItem(stretch);
        }
        items[static_cast<QPushButton*>(QObject::sender())]->show();

    }
}

TabListWidget::Items::iterator TabListWidget::current()
{
    Items::iterator it(items.begin());
    while (it != items.end() && it.value()->isHidden()) {
        ++it;
    }
    return it;
}

int	TabListWidget::addTab(QWidget* page, const QString& label)
{
    return addTab(items.insert(new QPushButton(label, this), page));
}

int	TabListWidget::addTab(QWidget* page, const QIcon& icon, const QString& label)
{
    return addTab(items.insert(new QPushButton(icon, label, this), page));
}
