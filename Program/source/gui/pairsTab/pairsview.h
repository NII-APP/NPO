#ifndef PAIRSVIEW_H
#define PAIRSVIEW_H

#include <QTreeView>
class PairModel;
class Project;

class PairsView : public QTreeView
{
    Q_OBJECT
    PairModel* myModel() const;

    void mousePressEvent(QMouseEvent *event);

    void setModel(QAbstractItemModel *model) { QTreeView::setModel(model);  }
public:
    explicit PairsView(QWidget *parent = 0);

signals:
    void addPairPressed();

public slots:
    void acceptNewProject();
};

#endif // PAIRSVIEW_H
