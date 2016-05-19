#ifndef FEMVIEWER_H
#define FEMVIEWER_H

#include <QWidget>

class FEMWidget;
class FEM;
class QAction;
class EigenMode;
class ToolBar;

class FEMViewer : public QWidget
{
    Q_OBJECT

    FEMWidget* const femWidget;
    ToolBar* const toolbox;

    bool eventFilter(QObject *, QEvent *);

    void resizeEvent(QResizeEvent*);
    void leaveEvent(QEvent *);

    void paintEvent(QPaintEvent *);

    QSize sizeHint() const;
public:
    struct FEMViewerState;

    FEMViewer(QWidget* parent = 0);
    ~FEMViewer();

    void setModel(const FEM*) const;
    const FEM* getModel() const;
    void setMode(const int m);
    void setProxyMode(const EigenMode&);

signals:

public slots:
    void colorize(int);
    void updateToolBar() const;
};

struct FEMViewerState {

};


#endif // FEMVIEWER_H
