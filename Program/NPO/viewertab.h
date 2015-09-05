#ifndef VIEWERTAB_H
#define VIEWERTAB_H

#include <QSplitter>
class ViewerView;
class FEM;
class FEMWidget;
#include <QModelIndex>

class FEMViewer;

class ViewerTab : public QSplitter
{
    Q_OBJECT
    ViewerView* const femView;
    FEMViewer* const femWidget;

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
    void addModel();
    void addModes(int meshId);
    void resetListView() {}
    void setMode(int);
    void setModel(const FEM *);
    void setModel(int id);
};

#endif // VIEWERTAB_H
