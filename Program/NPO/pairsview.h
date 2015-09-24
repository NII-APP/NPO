#ifndef PAIRSVIEW_H
#define PAIRSVIEW_H

#include <QTreeView>
class PairModel;
class Project;

class PairsView : public QTreeView
{
    Q_OBJECT
    PairModel* myModel() const;

public:
    explicit PairsView(QWidget *parent = 0);

    void setProject(const Project*);
};

#endif // PAIRSVIEW_H
