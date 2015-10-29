#include "femwidget.h"
#include <QApplication>
#define _USE_MATH_DEFINES
#include <cmath>

const float FEMWidget::AnimationOptions::doublePi = acos(-1.0f) * 2;

FEMWidget::FEMWidget(QWidget *parent)
    : CGLWidget(parent)
    , currentMode(-1)
    , animation(new AnimationOptions(this))
    , repaintLoop(new QTimer(this))
    , shader(nullptr)
{
    connect(repaintLoop, SIGNAL(timeout()), this, SLOT(repaint()));
    repaintLoop->setSingleShot(true);
#ifdef QT_NO_DEBUG
    repaintLoop->setInterval(1000 / 25);
#else
    repaintLoop->setInterval(10);
#endif
}

FEMWidget::~FEMWidget()
{
    animation->desertParent(this);
    if (!animation->isHaveParen()) {
        delete animation;
    }
}

void FEMWidget::initializeCGL()
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
    repaintLoop->start();
}
void FEMWidget::setData(const FEM* model)
{
    qDeleteAll(meshes);
    meshes.clear();
    if (!shader) {
        this->initializeGL();
    }

    if (model) {
        this->makeCurrent();
        meshes << new MeshBuffer(model, shader, this);
        scene() = model->getBox();
    }
}

void FEMWidget::setData(const QList<const FEM *> &models)
{
    qDeleteAll(meshes);
    meshes.clear();
    if (!shader) {
        this->initializeGL();
    }

    this->makeCurrent();
    scene() = CParallelepiped();
    for (auto model : models) {
        meshes.push_back(new MeshBuffer(model, shader, this));

        scene() |= model->getBox();
    }
}

void FEMWidget::drawUnityQuad() {
    drawGradienQuad(0xd9dbdc, 0xd9dbdc, 0xa0aa9a, 0xa0aa9a);
}

void FEMWidget::paintCGL()
{
    QTime now(QTime::currentTime());
    for (MeshBuffer* item : meshes) {
        item->bind();
        shader->setUniformValue("k", static_cast<GLfloat>(item->fem()->getBox().size() / 10.0f * item->getCurrentDefoultMagnitude() * animation->now(now)));
        item->fem()->render();
        item->fem()->renderNet();
        item->release();
    }
    repaintLoop->start();
    //to save background unity quad
    shader->setUniformValue("k", 0.0f);
}

FEMWidget::MeshBuffer::MeshBuffer(const FEM* data, QOpenGLShaderProgram* shader, QObject* parent)
    : QObject(parent)
    , self(data)
    , vertex(QOpenGLBuffer::VertexBuffer)
    , array(new QOpenGLVertexArrayObject(this))
    , vertexSize(static_cast<int>(data->getNodes().size()) * sizeof(CGL::CVertexes::value_type))
    , colorsSize(static_cast<int>(data->getNodes().size()) * sizeof(CGL::Colors::value_type))
    , proxyDefoultMagnitude(0.0f)
{
    array->create();
    array->bind();
    if (!vertex.isCreated()) {
        vertex.create();
    }
    vertex.bind();
    vertex.allocate(vertexSize + colorsSize + vertexSize);
    vertex.write(0, static_cast<const void*>(data->getNodes().data()), vertexSize);
    vertex.write(vertexSize, static_cast<const void*>(data->getColors().data()), static_cast<int>(data->getColors().size()));
    shader->setAttributeBuffer("in_Position", GL_FLOAT, 0, 3);
    shader->enableAttributeArray("in_Position");

    shader->setAttributeBuffer("in_Color", GL_UNSIGNED_BYTE, vertexSize, 3);
    shader->enableAttributeArray("in_Color");
    shader->setAttributeBuffer("in_Form", GL_FLOAT, vertexSize + colorsSize, 3);
    shader->enableAttributeArray("in_Form");
    setCurrentMode(0);
    array->release();
}

void FEMWidget::setMode(int m) {
    currentMode = m;
    for (MeshBuffer* mesh : meshes) {
        mesh->setCurrentMode(m);
    }
}

void FEMWidget::colorize(int m) {
    for (MeshBuffer* i : meshes) {
        i->colorize(m);
    }
}

void FEMWidget::setMagnitude(double v) {
    animation->setMagnitude(v);
}

void FEMWidget::setFrequency(double v) {
    animation->setFrequency(v);
}

void FEMWidget::pause() {
    animation->pause();
}

void FEMWidget::play() {
    animation->play();
}

void FEMWidget::MeshBuffer::setCurrentMode(int form) {
    if (modes().size() > form && form >= 0) {
        proxyDefoultMagnitude = 0.0f;
        colorize(form);
        mode = form;
        array->bind();
        vertex.bind();
        vertex.write(colorsSize + vertexSize, modes()[form].form().data(), vertexSize);
        array->release();
    }
}

void FEMWidget::MeshBuffer::setProxyMode(const EigenMode& imposter) {
    proxyDefoultMagnitude = imposter.defoultMagnitude();
    self->colorize(imposter.form());
    array->bind();
    vertex.bind();
    vertex.write(colorsSize + vertexSize, imposter.form().data(), vertexSize);
    uploadColors();
    array->release();
}

void FEMWidget::reloadColors(const FEM* w) {
    MeshBuffers::iterator b(meshes.begin());
    while (b != meshes.end() && (*b)->fem() != w) {
        ++b;
    }
    (*b)->uploadColors();
}

void FEMWidget::MeshBuffer::colorize(int m) {
    if (m >= 0 && m < self->getModes().size()) {
        self->colorize(m);
        uploadColors();
    }
}

void FEMWidget::MeshBuffer::uploadMode() {
    array->bind();
    vertex.bind();
    vertex.write(colorsSize + vertexSize, modes()[mode].form().data(), vertexSize);
    array->release();
}
void FEMWidget::MeshBuffer::uploadColors() {
    array->bind();
    vertex.bind();
    vertex.write(vertexSize, static_cast<const void*>(self->getColors().data()), static_cast<int>(self->getColors().size() * sizeof(char)));
    array->release();
}
void FEMWidget::MeshBuffer::uploadVertexes() {
    array->bind();
    vertex.bind();
    vertex.write(0, static_cast<const void*>(self->getNodes().data()), vertexSize);
    array->release();
}

QList<const FEM *> FEMWidget::getData() const
{
    QList<const FEM*> result;
    for (const MeshBuffer* b : meshes) {
        result.push_back(b->fem());
    }
    return result;
}

void FEMWidget::setProxyMode(const EigenMode& imposter) {
    for (MeshBuffer* b : meshes) {
        if (imposter.size() <= b->fem()->getNodes().size()) {
            this->makeCurrent();
            b->setProxyMode(imposter);
        }
    }
}

void FEMWidget::wheelEvent(QWheelEvent * e) {
    if (e->modifiers() & Qt::CTRL) {
        animation->multMagnitude(e->delta() > 0 ? 0.8 : 1.2);
        magnitudeChanged(animation->getMagnitude());
        this->repaint();
    }
    if (e->modifiers() & Qt::ALT) {
        animation->multFrequency(e->delta() > 0 ? 0.8 : 1.2);
        frequencyChanged(animation->getFrequency());
        this->repaint();
    }
    if (!(e->modifiers() & (Qt::ALT | Qt::CTRL))) {
        CGLWidget::wheelEvent(e);
    }
}

FEMWidget::AnimationOptions::AnimationOptions(FEMWidget *parent)
    : magnitude(1.0f)
    , frequency(1.0f)
    , initialTime(QTime::currentTime())
    , initialPhase(0.0f)
    , pauseFrequency(std::numeric_limits<float>::quiet_NaN())
{
    parents.push_back(parent);
}

void FEMWidget::AnimationOptions::addParent(const FEMWidget * w) {
    parents.push_back(w);
}

void FEMWidget::AnimationOptions::desertParent(const FEMWidget *w) {
    if (!parents.removeOne(w)) {
        qWarning() << w->objectName() << " GeometryWidget try remove hear from animationoptions parents list but was not stored in it";
    }
}

void FEMWidget::AnimationOptions::setFrequency(float v) {
    const QTime t(QTime::currentTime());
    initialPhase = phase(t);
    initialTime = t;
    frequency = v;
}

bool FEMWidget::AnimationOptions::isHaveParen() const {
    return !parents.isEmpty();
}


double FEMWidget::MeshBuffer::getDefaultMagnitude() const {
    try {
        return modes()[mode].defoultMagnitude();
    } catch (const std::out_of_range&) {
        return 0.0;
    }
}
