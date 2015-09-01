#ifndef VIEWERVIEW_H
#define VIEWERVIEW_H

#include <QTreeView>

class ViewerView : public QTreeView
{
    Q_OBJECT
public:
    ViewerView(QWidget* parent = 0);

signals:
    void addModelPressed();

public slots:
    void currentChanged(const QModelIndex & current, const QModelIndex & previous);
};

#endif // VIEWERVIEW_H
