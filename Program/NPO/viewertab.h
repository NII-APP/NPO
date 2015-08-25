#ifndef VIEWERTAB_H
#define VIEWERTAB_H

#include <QSplitter>
class ViewerView;
class FEM;
class FEMWidget;
#include <QModelIndex>

class ViewerTab : public QSplitter
{
    Q_OBJECT
    ViewerView* femView;

public:
    enum EnapledItems {
        MACChart = 0x1,
        MeshPane = 0x2,
        MeshList = 0x4,
        AnimationPane = 0x8,
        FormSpinner = 0x10
    };

    explicit ViewerTab(QWidget *parent = 0);
    ~ViewerTab();

signals:

public slots:
    void addModel() {}
    void resetListView() {}
    void setMode(int) {}
    void setMesh(const FEM *) {}
private slots:
    void listPatrol(QModelIndex);
};

#endif // VIEWERTAB_H
