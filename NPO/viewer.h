#ifndef VIEWER_H
#define VIEWER_H

#include <QSplitter>
#include <QModelIndex>
#include <QListView>
#include "engine/CGL/ccolumnchart.h"
#include <QSpinBox>
#include <QLabel>
class MeshScene;
class Geometry;

class Viewer : public QSplitter
{
    Q_OBJECT
    MeshScene* geometryWidget;
    QListView* geometriesView;
    CGL::CColumnChart* macChart;

    QFrame* formSelector;
    QLabel* formLabel;
    QLabel* formSubLabel;
    QSpinBox* form;

    bool colorizeBundle;

    enum EnapledItems {
        MAC_chart,
        Geometry_widget,
        Models_list
    };

public:
    explicit Viewer(QWidget *parent = 0);
    ~Viewer();

signals:

public slots:
    void addModel();
    void resetListView();
    void setMode(int);
    void setMesh(Geometry* g);
private slots:
    void listPatrol(QModelIndex);
};

#endif // VIEWER_H
