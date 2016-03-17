#ifndef FEMVIEWER_H
#define FEMVIEWER_H

#include <QWidget>
#include <QIcon>

class QToolBar;
class FEMWidget;
class FEM;
class QAction;
class EigenMode;

class FEMViewer : public QWidget
{
    Q_OBJECT

    class FEMViewerModeInput;
    class FEMViewerFrequencyInput;
    class FEMViewerMagnitudeInput;

    FEMWidget* const femWidget;
    QToolBar* const toolbox;

    FEMViewerModeInput* const mode;
    FEMViewerFrequencyInput* const frequency;
    FEMViewerMagnitudeInput* const magnitude;

    const QIcon pause;
    const QIcon play;
    QAction* run;
    QAction* stop;

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

private slots:
    void runTrigger();
    void runEnable();
    void stopTrigger(bool);
};

struct FEMViewerState {

};


#endif // FEMVIEWER_H
