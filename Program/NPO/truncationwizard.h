#ifndef TRUNCATIONWIZARD_H
#define TRUNCATIONWIZARD_H

#include <QDialog>
#include "engine/fempair.h"
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

    FEMPair* current;

    TruncationWizard(QWidget* parent = 0);
    ~TruncationWizard();
public:

    static FEMPair* exec(QWidget* parent);

private slots:
    void previewPatrol();

public slots:
    void newMac(const FEMPair::Relation& r) {
        current->makeMac(r);
        chart->setData(current->getMac());
    }
};

class TruncationWizard::Preview : public QWidget {
    Q_OBJECT

    QComboBox* const selector;
    FEMViewer* const screen;
    std::vector<const FEM*> meshes;
public:
    Preview(Qt::ToolBarArea area, QWidget* parent = 0);

    const FEM* current() const;
signals:
    void meshSelected(int id);
    void meshSelected(const FEM*);

private slots:
    void selectorPatrol();
};

#endif // TRUNCATIONWIZARD_H
