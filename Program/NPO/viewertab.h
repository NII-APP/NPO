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
    ViewerNode* const cascadeNode;

    //you can see MAC when it still not estimated. this map allow to find coresponding widget to update it
    typedef QMap<FEM*, MACDisplay*> MACMap;
    MACMap MACs;

    typedef QMap<FEM*, FEMProcessor*> FEMProcessors;
    FEMProcessors processors;

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
    void acceptNewProject();
    void setMode(int);
    void setModel(const FEM *);
    void setModel(int id);
    void showMAC(int id);

private slots:
    void fogiveMACWidget();
    void fogiveFEMProcessor();
    void updateMACWidget();
};

#endif // VIEWERTAB_H
