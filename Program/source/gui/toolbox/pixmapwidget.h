#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QWidget>

class PixmapWidget : public QWidget
{
    void paintEvent(QPaintEvent *);
public:

    explicit PixmapWidget(QWidget *parent = 0);
    PixmapWidget(const QPixmap& img, QWidget* parent = 0);
    ~PixmapWidget();

    void setImage(const QImage&);
    void setPixmap(const QPixmap&);
    void setIcon(const QIcon&, const QSize commendableSize = QSize());
    const QPixmap& pixmap() const;

private:
    QPixmap __img;
};

#endif // ICONWIDGET_H
