#ifndef PAIRSTAB_H
#define PAIRSTAB_H

#include <QSplitter>
class PairsView;
class FEMPairViewer;

class PairsTab : public QSplitter
{
    Q_OBJECT
public:
    explicit PairsTab(QWidget *parent = 0);

public slots:
    void acceptNewProject();

private:
    PairsView* const __view;
    FEMPairViewer* const __viewer;
};

#endif // PAIRTAB_H
