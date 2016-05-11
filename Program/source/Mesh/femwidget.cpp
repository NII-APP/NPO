#include "femwidget.h"
#include <QApplication>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>

const float FEMWidget::AnimationOptions::doublePi = acos(-1.0f) * 2;
const QRgb FEMWidget::DEFAULT_COLOR = qRgb(0x00,0xFF,0x88);

FEMWidget::FEMWidget(QWidget *parent)
    : CGLWidget(parent)
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
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/scripts/meshSummator.vert")) {
#ifndef QT_NO_DEBUG
        //0x1804a4e90
        qDebug() << "log:" << shader->log();
        qDebug() << "come from GLSL version" << glGetString(GL_SHADING_LANGUAGE_VERSION_ARB);
        qFatal("fatal shader eror");
#endif
    }
    if (!shader->bind()) {
#ifndef QT_NO_DEBUG
        qFatal(shader->log().toLocal8Bit());
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
        if (meshes.first()->getCurrentMode() < model->getModes().size()) {
            meshes.first()->colorize(model->getModes().at(meshes.first()->getCurrentMode()).form());
        } else {
            meshes.first()->colorize(DEFAULT_COLOR);
        }
        scene() = model->getBox();
    }
}


void FEMWidget::colorize(const CArray &v, const QString& mes, const FEM* const mesh)
{
    measurment = mes;

    for (MeshBuffer* const bufer : meshes) if ((mesh == nullptr && bufer->fem()->getNodes().length() ==  v.size()) || bufer->fem() == mesh) {
        if (static_cast<int>(v.size()) != mesh->getNodes().length()) {
            bufer->colorize(DEFAULT_COLOR);
        } else {
            bufer->colorize(v);
        }
    }
}

void FEMWidget::colorize(const CVertexes& v, const QString& mes, const FEM* const mesh)
{
    measurment = mes;

    for (MeshBuffer* const bufer : meshes) if ((mesh == nullptr && bufer->fem()->getNodes().size() ==  v.size()) || bufer->fem() == mesh) {
        if (static_cast<int>(v.size()) != mesh->getNodes().length()) {
            bufer->colorize(DEFAULT_COLOR);
        } else {
            bufer->colorize(v);
        }
    }
}

void FEMWidget::colorize(int m, const QString & mes, const FEM* const mesh)
{
    measurment = mes;
    for (MeshBuffer* i : meshes) if ((mesh == nullptr && i->fem()->getModes().size() > m) || mesh == i->fem()) {
        if (i->fem()->getModes().size() > m) {
            i->colorize(i->fem()->getModes().at(m).form());
        } else {
            i->colorize(DEFAULT_COLOR);
        }
    }
}

void FEMWidget::colorizeElements(const CArray &, const QString&, const FEM* const)
{
    qFatal("colorize elements not supported");
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
    , vertexSize(static_cast<int>(data->getNodes().size()) * sizeof(CVertexes::value_type))
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
    shader->setAttributeBuffer("in_Position", GL_FLOAT, 0, 3);
    shader->enableAttributeArray("in_Position");

    shader->setAttributeBuffer("in_Color", GL_UNSIGNED_BYTE, vertexSize, 3);
    shader->enableAttributeArray("in_Color");
    shader->setAttributeBuffer("in_Form", GL_FLOAT, vertexSize + colorsSize, 3);
    shader->enableAttributeArray("in_Form");
    setCurrentMode(0);
    array->release();
}

void FEMWidget::setMode(int m, const FEM * const mesh)
{
    for (MeshBuffer* buf : meshes) if (mesh == nullptr || mesh == buf->fem()) {
        buf->setCurrentMode(m);
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

void FEMWidget::MeshBuffer::setCurrentMode(int form)
{
    if (modes().size() > form && form >= 0 && modes()[form].form().size() >=
            vertexSize / sizeof(CVertexes::value_type)) {
        proxyDefoultMagnitude = 0.0f;
        colorize(form);
        mode = form;
        array->bind();
        vertex.bind();
        vertex.write(colorsSize + vertexSize, modes()[form].form().data(), vertexSize);
        array->release();
    }
}

void FEMWidget::MeshBuffer::setProxyMode(const EigenMode& imposter)
{
#ifndef QT_NO_DEBUG
    qDebug() << "let's set FEMWidget::MeshBuffer::proxy mode" << imposter.size() << self->getNodes().size();
#endif
    proxyDefoultMagnitude = imposter.defoultMagnitude();
    array->bind();
    vertex.bind();
    vertex.write(colorsSize + vertexSize, imposter.form().data(), vertexSize);
    colorize(imposter.form());
    array->release();
#ifndef QT_NO_DEBUG
    qDebug() << "\tfine";
#endif
}

void FEMWidget::MeshBuffer::uploadMode() {
    array->bind();
    vertex.bind();
    vertex.write(colorsSize + vertexSize, modes()[mode].form().data(), vertexSize);
    array->release();
}

void FEMWidget::MeshBuffer::uploadColors(const CGL::Colors& colors)
{
    array->bind();
    vertex.bind();
    vertex.write(vertexSize, static_cast<const void*>(colors.data()), std::min(static_cast<int>(colors.size() * sizeof(CGL::Colors::value_type)), colorsSize));
    array->release();
}

void FEMWidget::MeshBuffer::colorize(const QRgb v)
{
    CGL::Colors data(colorsSize / sizeof(CGL::Colors::value_type));
    const unsigned char r[] = { qRed(v), qGreen(v), qBlue(v) };
    const unsigned char* it(r);
    const unsigned char* const end(r + 3);
    for (auto& i : data) {
        i = *it;
        ++it;
        if (it == end) {
            it = r;
        }
    }
    uploadColors(data);
}

void FEMWidget::MeshBuffer::colorize(const CVertexes& v)
{
    CRealRange range(v.estimateRange().flatProof());

    const float height(range.range());
    const float heightp2(height / 2.0f);
    const float heightp4(height / 4.0f);
    const float height3p4(height / 4.0f * 3.0f);

    assert(v.length() * 3 * sizeof(CGL::Colors::value_type) == colorsSize);
    CGL::Colors colors(std::min(v.size(), colorsSize / sizeof(CGL::Colors::value_type)));
    CGL::Colors::iterator i(colors.begin());
    for (int j(0), length(static_cast<int>(colors.size() / 3)); j != length; ++j)
    {
        const float z(v(j).length());
        unsigned char r, g, b;
        if(z <= 0) {
            r = g = 0;
            b = 255;
        }
        else if(z >= height) {
            r = 255;
            g = b = 0;
        }
        else
        {
            b = z <= heightp4 ? 255 : z < heightp2 ? (heightp2 - z ) * 1024.0f / height : 0;
            g = z < heightp4 ? z * 1024.0f / height : z <= height3p4 ? 255 : ( height - z ) * 1024.0f / height;
            r = z < heightp2 ? 0 : z < height3p4 ? ( z - heightp2 ) * 1024.0f / height : 255;
        }
        *i = r;
        ++i;
        *i = g;
        ++i;
        *i = b;
        ++i;
    }

    uploadColors(colors);
}

void FEMWidget::MeshBuffer::colorize(const CArray& v)
{
    CRealRange range(v.estimateRange());

    range.flatProof();

    const float minV(range.getMin());
    const float height(range.range());
    const float heightp2(height / 2.0f);
    const float heightp4(height / 4.0f);
    const float height3p4(height / 4.0f * 3.0f);

    assert(v.size() * sizeof(CGL::Colors::value_type) * 3 == colorsSize);
    CGL::Colors colors(v.size() * 3);
    CGL::Colors::iterator i(colors.begin());
    for (CArray::const_iterator it(v.begin()), end(v.end()); it != end; ++it)
    {
        const float z(*it - minV);
        unsigned char r, g, b;
        if (z != z) {
            r = g = b = 0;
        } else if (z <= 0) {
            r = g = 0;
            b = 255;
        } else if (z >= height) {
            r = 255;
            g = b = 0;
        } else {
            b = z <= heightp4 ? 255 : z < heightp2 ? (heightp2 - z ) * 1024.0f / height : 0;
            g = z < heightp4 ? z * 1024.0f / height : z <= height3p4 ? 255 : ( height - z ) * 1024.0f / height;
            r = z < heightp2 ? 0 : z < height3p4 ? ( z - heightp2 ) * 1024.0f / height : 255;
        }
        *i = r;
        ++i;
        *i = g;
        ++i;
        *i = b;
        ++i;
    }

    uploadColors(colors);
}

void FEMWidget::MeshBuffer::uploadVertexes()
{
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

void FEMWidget::setProxyMode(const EigenMode& imposter, const FEM * const mesh)
{
    for (MeshBuffer* b : meshes) if (mesh == nullptr || b->fem() == mesh) {
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
