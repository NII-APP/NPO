#ifndef PAIRSTAB_H
#define PAIRSTAB_H

#include <QSplitter>
class PairsView;
class PairViewer;

class PairsTab : public QSplitter
{
    Q_OBJECT
public:
    explicit PairsTab(QWidget *parent = 0);

public slots:
    void acceptNewProject();
    void addPair();

private:
    PairsView* const __view;
    PairViewer* const __viewer;
};

#endif // PAIRTAB_H
