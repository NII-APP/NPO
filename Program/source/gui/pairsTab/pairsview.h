#ifndef PAIRSVIEW_H
#define PAIRSVIEW_H

#include <QTreeView>
class PairModel;
class Project;

class PairsView : public QTreeView
{
    Q_OBJECT
    PairModel* myModel() const;
    int currentPair;

    void mousePressEvent(QMouseEvent *event);

    void setModel(QAbstractItemModel *model) { QTreeView::setModel(model);  }

    void currentChanged(const QModelIndex & current, const QModelIndex & prew);
public:
    explicit PairsView(QWidget *parent = 0);

    PairModel* model() const;

signals:
    void addPairPressed();
    void currentPairChanged(int);

public slots:
    void acceptNewProject();
};

#endif // PAIRSVIEW_H
