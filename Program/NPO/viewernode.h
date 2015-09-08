#ifndef VIEWERNODE_H
#define VIEWERNODE_H

#include <QSplitter>

class FEMViewer;
class FEM;
namespace CGL { class C3dColumnChart; }

class ViewerNode : public QSplitter
{
    Q_OBJECT
public:
    explicit ViewerNode(QWidget *parent = 0);


signals:

public slots:
    void setModel(const FEM* const model);
    void setMode(int);
    void update();

private:
    FEMViewer* const __viewer;
    CGL::C3dColumnChart* const __macChart;
};

#endif // VIEWERNODE_H
