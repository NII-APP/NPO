#ifndef VIEWERVIEW_H
#define VIEWERVIEW_H

#include <QTreeView>

class ViewerModel;

class ViewerView : public QTreeView
{
    Q_OBJECT
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

public slots:
    void update();
    void acceptNewProject();

public slots:
    void currentChanged(const QModelIndex & current, const QModelIndex & previous);
};

#endif // VIEWERVIEW_H
