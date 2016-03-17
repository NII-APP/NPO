#ifndef VIEWERNODE_H
#define VIEWERNODE_H

#include <QSplitter>

class FEMViewer;
class FEM;
namespace CGL { class C3dColumnChart; }

#define MAC_DISABLE

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
#ifndef MAC_DISABLE
    CGL::C3dColumnChart* const __macChart;
#endif
};

#endif // VIEWERNODE_H
