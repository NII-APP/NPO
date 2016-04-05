#ifndef VIEWERTAB_H
#define VIEWERTAB_H

#include <QSplitter>
class ViewerView;
class FEM;
class FEMWidget;
class ViewerNode;
#include <QModelIndex>

class FEMViewer;
class MACDisplay;
class FEMProcessor;

class ViewerTab : public QSplitter
{
    Q_OBJECT
    ViewerView* const femView;
    /// @todo wrap the ViewerNode to SplitCascade
    FEMViewer* const cascadeNode;

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
    void acceptNewProject();
    void setMode(int);
    void setModel(const FEM *);
    void setModel(int id);
    void addModel();
};

#endif // VIEWERTAB_H
