#ifndef FEMWIDGET_H
#define FEMWIDGET_H

#include "cglwidget.h"
#include "fem.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QWheelEvent>
#include <cparallelepiped.h>

class FEMWidget : public CGLWidget
{
    class MeshBuffer;
public:
    class AnimationOptions;
private:
    typedef QList<MeshBuffer*> MeshBuffers;
    MeshBuffers meshes;
    QOpenGLShaderProgram* shader;
    int currentMode;
    QTimer* repaintLoop;

    AnimationOptions* animation;

    void wheelEvent(QWheelEvent * event);

protected:
    void initializeCGL();
    void paintCGL();
public:
    FEMWidget(QWidget* parent = 0);
    ~FEMWidget();

    AnimationOptions* getAnimationOptions() { return animation; }

    void setData(const FEM* model);
    void setData(const FEM& model) { setData(&model); }
    void setData(const QList<const FEM*>& model);
    QList<const FEM*> getData() const;

    void reloadColors(const FEM* forWho);
public slots:
    void setMode(const int);
};

class FEMWidget::MeshBuffer : public QObject
{
    const FEM* const self;
    QOpenGLBuffer vertex;
    QOpenGLVertexArrayObject* array;
    const int vertexSize;
    const int colorsSize;
    int mode;
    const EigenModes& modes() const { return self->getModes(); }
public:
    MeshBuffer(const FEM* data, QOpenGLShaderProgram* shader, QObject* parent);

    void bind() { array->bind(); }
    void release() { array->release(); }
    const FEM* fem() const { return self; }
    void setCurrentMode(int form);
    int getCurrentMode() const { return mode; }
    float getCurrentDefoultMagnitude() const { return self->getModes().at(mode).defoultMagnitude(); }
    double getDefaultMagnitude() const;

    void uploadMode();
    void uploadColors();
    void uploadVertexes();
};


class FEMWidget::AnimationOptions
{
///@todo introduce QSharedData inheritance
    float magnitude;
    float frequency;//hz
    QTime initialTime;
    float initialPhase;
    QList<const FEMWidget*> parents;

    static const float doublePi;
public:
    AnimationOptions(FEMWidget* parent);

    float getMagnitude() const { return magnitude; }
    float getFrequency() const { return frequency; }
    void setMagnitude(float v) { magnitude = v; }
    void setFrequency(float v);

    void multMagnitude(float v) { setMagnitude(magnitude * v); }
    void multFrequency(float v) { setFrequency(frequency * v); }

    float now() const { return now(QTime::currentTime()); }
    float phase(const QTime& t) const { return initialPhase + initialTime.msecsTo(t) / 1000.0f * doublePi * frequency; }
    float now(const QTime& t) const { return sin(phase(t)) * magnitude; }

    void addParent(const FEMWidget*);
    void desertParent(const FEMWidget*);
    bool isHaveParen() const;
};

#endif // FEMWIDGET_H
