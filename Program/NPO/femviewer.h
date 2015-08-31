#ifndef FEMVIEWER_H
#define FEMVIEWER_H

#include <QWidget>

class QToolBar;
class FEMWidget;

class FEMViewer : public QWidget
{
    Q_OBJECT

    FEMWidget* const femWidget;
    QToolBar* const toolbox;

    bool eventFilter(QObject *, QEvent *);

    void resizeEvent(QResizeEvent*);
    void leaveEvent(QEvent *);
public:
    struct FEMViewerState;

    FEMViewer(QWidget* parent = 0);

signals:

public slots:
};

struct FEMViewerState {

};


#endif // FEMVIEWER_H
