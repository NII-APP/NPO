#ifndef FEMSCREEN_H
#define FEMSCREEN_H

#include <QWidget>

class FEMWidget;
class ToolBar;

class FEMScreen : public QWidget
{
public:
    FEMScreen(QWidget* paretn = 0);
    ~FEMScreen();

    virtual void setMode(const int m);
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint();

protected:
    FEMWidget* const femWidget;
    ToolBar* const toolbox;

    virtual bool eventFilter(QObject *, QEvent *);

    virtual void resizeEvent(QResizeEvent*);
    virtual void leaveEvent(QEvent *);

    virtual void paintEvent(QPaintEvent *);
    void updateToolBar();
};

#endif // FEMSCREEN_H
