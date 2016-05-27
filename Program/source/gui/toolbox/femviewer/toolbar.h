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
    QAction* const lock;

    FEMWidget* const femWidget;
public:
    ToolBar(FEMWidget *femWidget, QWidget* parent = 0);
    ~ToolBar();

    bool isLocked() const;
    void refresh();
    void setProxyModeState();
    void setMode(const int);
    ModeInput* modeInput();
};

#endif // TOOLBAR_H
