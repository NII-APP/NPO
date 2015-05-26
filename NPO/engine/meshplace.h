#ifndef MESHPLACE_H
#define MESHPLACE_H

#include "CGL/cglwidget.h"
#include "geometryform.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class MeshPlace : public CGLWidget
{
    class MeshBuffer;
    typedef QVector<MeshBuffer*> MeshBuffers;
    MeshBuffers meshes;
    QOpenGLShaderProgram* shader;

protected:
    void initializeCGL();
    void paintCGL();
public:
    MeshPlace(QWidget* parent = 0);
    ~MeshPlace();

    void setData(const Mesh* model);
    void setData(const Mesh& model) { setData(&model); }
    void setData(const QVector<const Mesh*>& model);
    void setMode(const int);
    QVector<const Mesh*> getData() const;
};

class MeshPlace::MeshBuffer : public QObject
{
    const Mesh* const self;
    QOpenGLBuffer vertex;
    QOpenGLVertexArrayObject* array;
    const int vertexSize;
    const int colorsSize;
    int mode;
    const MeshModes* modes() const { return dynamic_cast<const MeshModes*>(self); }
public:
    MeshBuffer(const Mesh* data, QOpenGLShaderProgram* shader, QObject* parent);

    void bind() { array->bind(); }
    void release() { array->release(); }
    const Mesh* mesh() const { return self; }
    void setCurrentMode(int form);
    double GetDefaultMagnitude() const { return modes() ? modes()->getDefoultMagnitude(mode) : 0.0; }
};

#endif // MESHPLACE_H
