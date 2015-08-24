#ifndef VIEWER_H
#define VIEWER_H

#include <QSplitter>
#include <QModelIndex>
#include <QListView>
#include "c3dcolumnchart.h"
#include <QSpinBox>
#include <QLabel>
class MeshPlace;
class Mesh;

class Viewer : public QSplitter
{
    Q_OBJECT
    MeshPlace* geometryWidget;
    QListView* geometriesView;
    CGL::C3dColumnChart* macChart;

    QFrame* formSelector;
    QLabel* formLabel;
    QLabel* formSubLabel;
    QSpinBox* form;

    bool colorizeBundle;

public:
    enum EnapledItems {
        MACChart = 0x1,
        MeshPane = 0x2,
        MeshList = 0x4,
        AnimationPane = 0x8,
        FormSpinner = 0x10
    };

    explicit Viewer(QWidget *parent = 0, unsigned enabledItems = 0xFF);
    ~Viewer();

signals:

public slots:
    void addModel();
    void resetListView();
    void setMode(int);
    void setMesh(const FEM *g);
private slots:
    void listPatrol(QModelIndex);
};

#endif // VIEWER_H
