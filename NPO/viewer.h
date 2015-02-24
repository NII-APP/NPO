#ifndef VIEWER_H
#define VIEWER_H

#include <QSplitter>
#include <QModelIndex>
#include <QListView>
#include "engine/CGL/ccolumnchart.h"
class GeometryWidget;

class Viewer : public QSplitter
{
    Q_OBJECT
    GeometryWidget* geometryWidget;
    QListView* geometriesView;
    CGL::CColumnChart* macChart;

public:
    explicit Viewer(QWidget *parent = 0);
    ~Viewer();

signals:

public slots:
    void addModel();
    void resetListView();
private slots:
    void listPatrol(QModelIndex);
};

#endif // VIEWER_H
