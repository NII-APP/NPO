#ifndef TRUNCATIONWIZARD_H
#define TRUNCATIONWIZARD_H

#include <QDialog>
#include "engine/geometrypair.h"
#include <QComboBox>
#include "engine/geometrywidget.h"
#include <QVBoxLayout>
#include "application.h"
#include "project.h"

class TruncationWizard : public QDialog
{
    class Preview;
    Preview* first;
    Preview* second;

    TruncationWizard(QWidget* parent = 0);
    ~TruncationWizard();
public:

    static GeometryPair* exec(QWidget* parent);
};

class TruncationWizard::Preview : public QWidget {
    Q_OBJECT

    QComboBox* selector;
    GeometryWidget* screen;
    std::vector<GeometryForm* const> meshes;
public:
    Preview(Qt::ToolBarArea area, QWidget* parent = 0)
        : QWidget(parent)
        , selector(new QComboBox(this))
        , screen(new GeometryWidget(this))
    {
        this->setLayout(new QVBoxLayout);
        foreach (Geometry* const g, Application::project()->modelsList()) {
            GeometryForm* const item(dynamic_cast<GeometryForm*>(g));
            if (item && item->modesCount()) {
                meshes.push_back(item);
                selector->addItem(item->getName());
            }
        }
        if (Qt::BottomToolBarArea == area) {
            this->layout()->addWidget(screen);
            this->layout()->addWidget(selector);
            if (meshes.size() >= 2) {
                selector->setCurrentIndex(1);
            }
        } else {
            this->layout()->addWidget(selector);
            this->layout()->addWidget(screen);
        }
        this->connect(selector, SIGNAL(currentIndexChanged(int)), SLOT(selectorPatrol()));
        selectorPatrol();
    }

    GeometryForm* current() const {
        return selector->currentIndex() < meshes.size() && selector->currentIndex() >= 0
                ? meshes.at(selector->currentIndex()) : 0;
    }

private slots:
    void selectorPatrol() {
        screen->setModel(current());
    }
};

#endif // TRUNCATIONWIZARD_H
