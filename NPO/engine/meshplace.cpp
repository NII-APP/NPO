#include "meshplace.h"
#include <QApplication>

MeshPlace::MeshPlace(QWidget *parent)
    : CGLWidget(parent)
{
    shader = nullptr;
}

MeshPlace::~MeshPlace()
{

}

void MeshPlace::initializeCGL()
{
    if (shader) {
        return;
    }

    shader = new QOpenGLShaderProgram(this);
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/codes/meshSummator.vert")) {
#ifndef QT_NO_DEBUG
        qDebug() << "log:" << shader->log();
#endif
    }
    if (!shader->bind()) {
#ifndef QT_NO_DEBUG
        qDebug() << shader->log();
#endif
    }
    Q_ASSERT(shader->isLinked());
    glEnableClientState(GL_VERTEX_ARRAY);
    shader->setUniformValue("k", 2.0f);
}
void MeshPlace::setData(const Mesh* model)
{
    qDeleteAll(meshes);
    meshes.clear();
    if (!shader) {
        this->initializeGL();
    }

    meshes.resize(1);
    MeshBuffer*& item = (meshes.first());

    this->makeCurrent();
    item = new MeshBuffer(model, shader, this);
    scene() = model->getBox();
}

void MeshPlace::setData(const QVector<const Mesh*>& models)
{
    qDeleteAll(meshes);
    meshes.clear();
    if (!shader) {
        this->initializeGL();
    }

    this->makeCurrent();
    scene() = CParallelepiped();
    for (const Mesh* model : models) {
        meshes.push_back(new MeshBuffer(model, shader, this));

        scene() |= model->getBox();
    }
}

void MeshPlace::paintCGL()
{
    for (MeshBuffer* item : meshes) {
        item->bind();
        item->mesh()->render();
        item->mesh()->renderNet();
        item->release();
    }
}

MeshPlace::MeshBuffer::MeshBuffer(const Mesh* data, QOpenGLShaderProgram* shader, QObject* parent)
    : QObject(parent)
    , self(data)
    , vertex(QOpenGLBuffer::VertexBuffer)
    , array(new QOpenGLVertexArrayObject(this))
    , vertexSize(static_cast<int>(data->getNodes().size()) * sizeof(CGL::CVertexes::value_type))
    , colorsSize(static_cast<int>(data->getColors().size()) * sizeof(CGL::Colors::value_type))
{
    array->create();
    array->bind();
    if (!vertex.isCreated()) {
        vertex.create();
    }
    vertex.bind();
    vertex.allocate(vertexSize + colorsSize + vertexSize);
    vertex.write(0, static_cast<const void*>(data->getNodes().data()), vertexSize);
    vertex.write(vertexSize, static_cast<const void*>(data->getColors().data()), colorsSize);
    shader->setAttributeBuffer("in_Position", GL_FLOAT, 0, 3);
    shader->enableAttributeArray("in_Position");

    shader->setAttributeBuffer("in_Color", GL_UNSIGNED_BYTE, vertexSize, 3);
    shader->enableAttributeArray("in_Color");
    shader->setAttributeBuffer("in_Form", GL_FLOAT, vertexSize + colorsSize, 3);
    shader->enableAttributeArray("in_Form");
    setCurrentMode(3);
    array->release();
}

void MeshPlace::setMode(const int m) {
    for (MeshBuffer* mesh : meshes) {
        mesh->setCurrentMode(m);
    }
}

void MeshPlace::MeshBuffer::setCurrentMode(int form)
{
    if (modes() && form >= 0 && form < modes()->modesCount()) {
        mode = form;
        array->bind();
        vertex.bind();
        vertex.write(colorsSize + vertexSize, modes()->form(form).data(), vertexSize);
        array->release();
    }
}

QVector<const Mesh*> MeshPlace::getData() const
{
    QVector<const Mesh*> result;
    for (const MeshBuffer* b : meshes) {
        result.push_back(b->mesh());
    }
    return result;
}
