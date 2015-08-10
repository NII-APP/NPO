#ifndef TRUNCATIONWIZARD_H
#define TRUNCATIONWIZARD_H

#include <QDialog>
#include "engine/geometrypair.h"
#include <QComboBox>
#include <QVBoxLayout>
#include "application.h"
#include "project.h"
#include "relationdialog.h"
#include "c3dcolumnchart.h"

class TruncationWizard : public QDialog
{
    Q_OBJECT

    class Preview;
    Preview* first;
    Preview* second;

    RelationDialog* relation;

    CGL::C3dColumnChart* chart;

    MeshPair* current;

    TruncationWizard(QWidget* parent = 0);
    ~TruncationWizard();
public:

    static MeshPair* exec(QWidget* parent);

private slots:
    void previewPatrol();

public slots:
    void newMac(const MeshPair::Relation& r) {
        current->makeMac(r);
        chart->setData(current->getMac());
    }
};

class TruncationWizard::Preview : public QWidget {
    Q_OBJECT

    QComboBox* selector;
    Viewer* screen;
    std::vector<MeshForm* const> meshes;
public:
    Preview(Qt::ToolBarArea area, QWidget* parent = 0)
        : QWidget(parent)
        , selector(new QComboBox(this))
        , screen(new Viewer(this, Viewer::MeshPane | Viewer::FormSpinner | Viewer::AnimationPane))
    {
        this->setLayout(new QVBoxLayout);
        foreach (Mesh* const g, Application::project()->modelsList()) {
            MeshForm* const item(dynamic_cast<MeshForm*>(g));
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

    MeshForm* current() const
    {
        return selector->currentIndex() < meshes.size() && selector->currentIndex() >= 0
                ? meshes.at(selector->currentIndex()) : 0;
    }
signals:
    void meshSelected(int id);
    void meshSelected(MeshForm*);

private slots:
    void selectorPatrol()
    {
        MeshForm* c(current());
        screen->setMesh(c);
        emit meshSelected(c);

        const Project::Geometries& m(Application::project()->modelsList());
        int i(0);
        while (m.at(i) != c && m.size() > i) {
            ++i;
        }
        //emit meshSelected(i < m.size() ? i : -1);
    }
};

#endif // TRUNCATIONWIZARD_H
