#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QIcon>

class QAction;
class ModeInput;
class FrequencyInput;
class MagnitudeInput;
class FEMWidget;

class ToolBar : public QToolBar
{
    ModeInput* const mode;
    FrequencyInput* const frequency;
    MagnitudeInput* const magnitude;

    const QIcon pauseIcon;
    const QIcon playIcon;
    QAction* const run;
    QAction* const stop;

    FEMWidget* const femWidget;
public:
    ToolBar(FEMWidget *femWidget, QWidget* parent = 0);
    ~ToolBar();

    void refresh();
    void setProxyModeState();
    void setMode(const int);
};

#endif // TOOLBAR_H
