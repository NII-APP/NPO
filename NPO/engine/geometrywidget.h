#ifndef GEOMETRYWIDGET_H
#define GEOMETRYWIDGET_H
#include "cglwidget.h"
#include "geometryform.h"
#include "animationoptions.h"
#include <functional>
#include <QOpenGLShaderProgram>
class QAction;
class Geometry;

class GeometryWidget : public CGLWidget
{
    Q_OBJECT

    const Geometry* data;
    QAction* netAction;
    QAction* animationAction;
    QAction* pauseAction;
    int form;
    double initialPhase;
    QTime initialTime;
    int pauseTime;

    AnimationOptions* animation;

    QTimer* repaintLoop;

    QOpenGLShaderProgram* summator;

    //experemental function-style programing
    typedef std::function<void(GeometryWidget*)> VoidFunction;
    VoidFunction paintDisable;

    void paintCGL();
    void initializeCGL();
    void timerEvent(QTimerEvent *);

    double currentPhase() const;

    bool isAnimation() const;
    void drawIdantityQuad();

public:
    GeometryWidget(QWidget* parent = 0, QGLWidget* sharedWidget = 0, Qt::WindowFlags f = 0);
    ~GeometryWidget();

    void setModel(const Geometry *);
    void setModel(const Geometry&);
    const Geometry* getModel() { return data; }
    void setForm(int f);
    int getForm() const { return form; }

    void setDisablePaintFunction(VoidFunction);

public slots:
    //animation properties access
    AnimationOptions* getAnimationOptions() { return animation; }
    void setMagnitude(float v) const { animation->setMagnitude(v); }
    float getMagnitude() const { return animation->getMagnitude(); }
    void magnitudeIncr() const { animation->multMagnitude(1.2f); }
    void magnitudeDecr() const { animation->multMagnitude(0.8f); }
    void setFrequency(float v) const { animation->setFrequency(v); }
    float getFrequency() const { return animation->getFrequency(); }
    void frequencyIncr();
    void frequencyDecr();
    void initialAnimation();
    void formIncr();
    void formDecr();
    void triggerNet() { this->repaint(); }
    void triggerAnimation();
    void triggerPause();
};

#endif // GEOMETRYWIDGET_H
