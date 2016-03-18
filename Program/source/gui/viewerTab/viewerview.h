#ifndef VIEWERVIEW_H
#define VIEWERVIEW_H

#include <QTreeView>

class ViewerModel;
class QPushButton;
class FEMProcessor;

class ViewerView : public QTreeView
{
    Q_OBJECT
    void mousePressEvent(QMouseEvent*);
    void currentChanged(const QModelIndex & current, const QModelIndex & previous);

private:
    ViewerModel* myModel() const;
public:
    ViewerView(QWidget* parent = 0);

signals:
    void currentModelChanged(int);
    void currentModeChanged(int, int);
    void modesIdentificationPressed(int);
    void MACPressed(int, FEMProcessor*);
    void calcModes(int);

public slots:
    void addFEM();
    void importModes(int id);

    void update();
    void acceptNewProject();

private:
    typedef QMap<int, FEMProcessor*> Processors;
    Processors buf;
};

#endif // VIEWERVIEW_H
