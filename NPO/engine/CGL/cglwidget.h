#ifndef CGLWIDGET_H
#define CGLWIDGET_H

#include <QGLWidget>
#include <QMatrix4x4>
#include "cparallelepiped.h"
#include <QMenu>

class CGLWidget : public QGLWidget
{
    Q_OBJECT

    //min(width, heoght)
    int edge;
    QPair<GLfloat, GLfloat> translate;
    //current rotate
    QPair<GLfloat, GLfloat> rotate;
    //accumulated rotate
    QMatrix4x4 rotateMatrix;
    //rotate muf with copy of rotateMatrix when popup menu selecting
    QMatrix4x4 rotateBuf;
    CParallelepiped sceneSize;
    GLfloat zoom;
    //preceding mouse position
    QPoint mPPosition;
    QList<QAction *> sidesMenu;

protected:
    QMenu* menu;

    void safelyUpdate() { update(); }//ну там всякие соображения... не придавайте значения... можно менять
    virtual void drawIdantityQuad() { }
    void drawGradienQuad(const QRgb c1, const QRgb c2, const QRgb c3, const QRgb c4);
    void loadRotate();

    void initializeGL();
    virtual void initializeCGL() {}
    void resizeGL(int,int);
    virtual void resizeCGL(int, int) {}
    void paintGL();
    virtual void paintCGL() {}

    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void wheelEvent(QWheelEvent* e);
    virtual void keyPressEvent(QKeyEvent *);
    void debugSpace(const CParallelepiped &s);
    void bufRotate();

public:
    void clear();
    static void drawAxes();

    CGLWidget(QWidget *parent = 0, const QGLWidget* shareWidget = 0, Qt::WindowFlags f = 0);

    void setScene(const CParallelepiped& v) { sceneSize = v; }
    CParallelepiped& scene() { return sceneSize; }
    const CParallelepiped& scene() const { return sceneSize; }


signals:

public slots:
    void transformIdentity();
    void initialZoom() { zoom = 0.5f; this->repaint(); }
    void zoomIn() { zoom *= 0.9f; this->repaint(); }
    void zoomOut() { zoom /= 0.9f; this->repaint(); }
    void centre() { translate.first = translate.second = 0.0f; this->repaint(); }
private slots:
    void menuHandling(QAction* a);
    void menuTriggering(QAction*);

};

#endif // CGLWIDGET_H
