#ifndef MAINTABBAR_H
#define MAINTABBAR_H

#include <QTabBar>

class MainTabBar : public QTabBar
{
    static const unsigned BASE_WIDTH;
    static const unsigned ICON_SIZE;
    static const unsigned ROW_HEIGHT;
    static const unsigned TOP_MARGIN;
    static const unsigned ARROW_HEIGHT;

    unsigned iconSize;
public:
    MainTabBar(QWidget* parent = 0);
    ~MainTabBar();

    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent *);
    QSize tabSizeHint(int index) const;
};

#endif // MAINTABBAR_H
