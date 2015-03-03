#ifndef TRUNCATIONTAB_H
#define TRUNCATIONTAB_H

#include <QMainWindow>

class TruncationTab : public QMainWindow
{
    Q_OBJECT

    QToolBar* bar;
public:
    explicit TruncationTab(QWidget *parent = 0);
    ~TruncationTab();

signals:

public slots:
    void addPair();
};

#endif // TRUNCATIONTAB_H
