#ifndef VIEWERVIEW_H
#define VIEWERVIEW_H

#include <QTreeView>

class ViewerModel;

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
    void addModelPressed();
    void currentModelChanged(int);
    void currentModeChanged(int, int);
    void importModesPressed(int);
    void modesIdentificationPressed(int);
    void MACPressed(int);
    void calcModes(int);

public slots:
    void update();
    void updateCurrentModel();
    void updateModel(int id);
    void acceptNewProject();
};

#endif // VIEWERVIEW_H
