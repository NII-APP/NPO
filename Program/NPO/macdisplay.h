#ifndef MACDISPLAY_H
#define MACDISPLAY_H

#include <QWidget>
#include <cmatrix.h>
#include <c3dcolumnchart.h>

class MACDisplay : public QWidget
{
    Q_OBJECT

    void closeEvent(QCloseEvent *);
public:
    explicit MACDisplay(QWidget *parent = 0);

signals:
    void closed();

public slots:
    void setData(const CGL::CMatrix& m);

private:
    CGL::C3dColumnChart* __chart;
};

#endif // MACDISPLAY_H
