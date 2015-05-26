#ifndef VIEWER_H
#define VIEWER_H

#include <QSplitter>
#include <QModelIndex>
#include <QListView>
#include "engine/CGL/ccolumnchart.h"
#include <QSpinBox>
#include <QLabel>
class MeshPlace;
class Geometry;

class Viewer : public QSplitter
{
    Q_OBJECT
    MeshPlace* geometryWidget;
    QListView* geometriesView;
    CGL::CColumnChart* macChart;

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
    void setMesh(const Geometry *g);
private slots:
    void listPatrol(QModelIndex);
};

#endif // VIEWER_H
