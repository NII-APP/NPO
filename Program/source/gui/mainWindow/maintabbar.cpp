#include "maintabbar.h"
#include <QDebug>
#include <QPainter>
#include <QAction>
#include <QIcon>
#include <QResizeEvent>

const unsigned MainTabBar::BASE_WIDTH = 70 / 70;
const unsigned MainTabBar::ICON_SIZE = 45;
const unsigned MainTabBar::ROW_HEIGHT = 30;
const unsigned MainTabBar::TOP_MARGIN = 0;
const unsigned MainTabBar::ARROW_HEIGHT = 10;

MainTabBar::MainTabBar(QWidget *parent)
    : QTabBar(parent)
{
}

void MainTabBar::resizeEvent(QResizeEvent* e) {
    iconSize = ICON_SIZE;
    QTabBar::resizeEvent(e);
}

void MainTabBar::paintEvent(QPaintEvent *) {
    QPainter p(this);

    QLinearGradient g(QPointF(0,0), QPointF(BASE_WIDTH,0));
    g.setSpread(QLinearGradient::PadSpread);
    g.setColorAt(0.0, QColor(170,170,170));
    g.setColorAt(1.0 - 1.0 / BASE_WIDTH, QColor(235,235,235));
    g.setColorAt(1.0, QColor(49,49,49));
    p.fillRect(QRect(QPoint(0,0),this->size()), g);

    QFont font;

    unsigned accumulator(TOP_MARGIN);
    for (int i(0); i != this->count(); ++i) {
        QSize size(tabSizeHint(i));
        if (this->currentIndex() == i) {
            QLinearGradient g(QPointF(0,0), QPointF(BASE_WIDTH,0));
            g.setSpread(QLinearGradient::PadSpread);
            g.setColorAt(0.0, QColor(64,64,64));
            g.setColorAt(1.0 - 1.0 / BASE_WIDTH, QColor(130,130,130));
            g.setColorAt(1.0, QColor(49,49,49));
            QPolygon polygon;
            polygon.reserve(6);
            if (i) {
                polygon.push_back(QPoint(0,accumulator - ARROW_HEIGHT / 2));
                polygon.push_back(QPoint(size.width() / 2,accumulator + ARROW_HEIGHT / 2));
                polygon.push_back(QPoint(size.width(),accumulator - ARROW_HEIGHT / 2));
            } else {
                polygon.push_back(QPoint(0,accumulator));
                polygon.push_back(QPoint(size.width(),accumulator));
            }
            polygon.push_back(QPoint(size.width(),accumulator + size.height() - ARROW_HEIGHT / 2));
            polygon.push_back(QPoint(size.width() / 2,accumulator + size.height() + ARROW_HEIGHT / 2));
            polygon.push_back(QPoint(0,accumulator + size.height() - ARROW_HEIGHT / 2));
            QPainterPath path;
            path.addPolygon(polygon);
            p.fillPath(path, g);
        }
        this->tabIcon(i).paint(&p,QRect(BASE_WIDTH / 2 - iconSize / 2, accumulator + ARROW_HEIGHT / 2,iconSize,iconSize));
        QTextOption options(Qt::AlignCenter);
        options.setWrapMode(QTextOption::WordWrap);
        p.drawText(QRect(0, iconSize + ARROW_HEIGHT / 2 + accumulator, BASE_WIDTH, ROW_HEIGHT),this->tabText(i),options);

        accumulator += size.height();
    }
}

QSize MainTabBar::tabSizeHint(int index) const {
    return QSize(BASE_WIDTH, ICON_SIZE + ROW_HEIGHT + TOP_MARGIN * !index + ARROW_HEIGHT + 4000 * !this->isTabEnabled(index));
}

MainTabBar::~MainTabBar()
{

}

