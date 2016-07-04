#ifndef VIEWERVIEW_H
#define VIEWERVIEW_H

#include <QTreeView>
#include "project/femmodel.h"

class FEMModel;
class QPushButton;
class EigenModesFounder;

class ViewerView : public QTreeView
{
    Q_OBJECT
    void mousePressEvent(QMouseEvent*);
    void currentChanged(const QModelIndex & current, const QModelIndex & previous);

private:
    FEMModel* myModel() const;
    void importModes(int id);
    void addFEM();
public:
    ViewerView(QWidget* parent = 0);

signals:
    void currentModelChanged(int);
    void currentModeChanged(int, int);
    void modesIdentificationPressed(int);
    void MACPressed(int, FEMModel::SharedEigenModesFounder);

public slots:

    void update();
    void acceptNewProject();

private:
    typedef QMap<int, FEMModel::SharedEigenModesFounder> Processors;
    Processors buf;
};

#endif // VIEWERVIEW_H
