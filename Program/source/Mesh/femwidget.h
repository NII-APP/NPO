#ifndef FEMWIDGET_H
#define FEMWIDGET_H

#include "cglwidget.h"
#include "fem.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QWheelEvent>
#include <cparallelepiped.h>
#include <QString>

class FEMWidget : public CGLWidget
{
    Q_OBJECT

    class MeshBuffer;
public:
    class AnimationOptions;
private:
    typedef QList<MeshBuffer*> MeshBuffers;
    MeshBuffers meshes;
    QOpenGLShaderProgram* shader;
    QTimer* repaintLoop;

    AnimationOptions* animation;

    QBitArray visible;

    void wheelEvent(QWheelEvent * event);

    //measurment of value which presented as color
    QString measurment;

    static const QRgb DEFAULT_COLOR;

    int toId(const FEM*) const;

protected:
    void initializeCGL();
    void paintCGL();
    void drawUnityQuad();
public:
    FEMWidget(QWidget* parent = 0);
    ~FEMWidget();

    AnimationOptions* getAnimationOptions() { return animation; }

    void setData(const FEM* model);
    void setData(const FEM& model) { setData(&model); }
    void setData(const QList<const FEM*>& model);
    QList<const FEM*> getData() const;
    void setProxyMode(const EigenMode &, const FEM* const mesh = nullptr);

    bool isVisible(const int modelId) const;
    bool isVisible(const FEM* const model) const;
    bool isVisible() const { return CGLWidget::isVisible(); }
    void setVisible(const bool, const int modelId);
    void setVisible(const bool, const FEM* const model);
    void setVisible(bool v) { CGLWidget::setVisible(v); }

    //estimate colors value as form interpolation in range [red : green : blue]
    void colorize(const CVertexes& v, const QString& mes = "", const FEM* const mesh = nullptr);
    void colorize(const CArray& v, const QString& mes = "", const FEM* const mesh = nullptr);
    void colorizeElements(const CArray &, const QString& = "", const FEM* const = nullptr);
    void colorize(int m, const QString& = "", const FEM* const mesh = nullptr);
    //void colorize(const int mode, const QString& meas= "", const FEM* const mesh = nullptr) const);
signals:
    void magnitudeChanged(double);
    void frequencyChanged(double);
public slots:
    void setMode(int, const FEM* const mesh = nullptr);
    void setMagnitude(double);
    void setFrequency(double);
    void pause();
    void play();
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
    float proxyDefoultMagnitude;
    QOpenGLShaderProgram* const shader;
public:
    MeshBuffer(const FEM* data, QOpenGLShaderProgram* shader, QObject* parent);

    void bind();
    void release() { array->release(); }
    const FEM* fem() const { return self; }
    void setCurrentMode(int form);
    int getCurrentMode() const { return mode; }
    float getCurrentDefoultMagnitude() const { try { return self->getModes().at(mode).defoultMagnitude(); } catch(...) { return proxyDefoultMagnitude; } }
    double getDefaultMagnitude() const;
    void colorize(const QRgb);
    void colorize(const CVertexes& v);
    void colorize(const CArray& v);
    void setProxyMode(const EigenMode& imposter);

    void uploadMode();
    void uploadColors(const Colors &);
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
    float pauseFrequency;//store frequency before pausing

    static const float doublePi;
public:
    AnimationOptions(FEMWidget* parent);

    float getMagnitude() const { return magnitude; }
    float getFrequency() const { return frequency; }
    void setMagnitude(float v) { magnitude = v; }
    void setFrequency(float v);
    bool isPaused() const {
        return pauseFrequency == pauseFrequency;
    }

    void pause() {
        if (pauseFrequency != pauseFrequency) {
            pauseFrequency = frequency;
            setFrequency(0.0f);
        }
    }
    void play() {
        if (pauseFrequency == pauseFrequency) {
            setFrequency(pauseFrequency);
            pauseFrequency = std::numeric_limits<float>::quiet_NaN();
        }
    }

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
