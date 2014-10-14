#include "geometrywidget.h"
#include "geometry.h"

GeometryWidget::GeometryWidget(QWidget *parent, QGLWidget *shareWidget, Qt::WindowFlags f)
    : CGLWidget(parent, shareWidget, f)
    , repaintLoop(new QTimer(this))
    , animation(new AnimationOptions(this))
{
    initialPhase = 0.0f;
    initialTime = QTime::currentTime();
    net = true;
    form = 0;
    repaintLoop->setInterval(10);
    this->connect(repaintLoop, SIGNAL(timeout()), SLOT(repaint()));

    this->menu->addSeparator();
    this->menu->addAction(tr("magnitude increment"), this, SLOT(magnitudeIncr()), QKeySequence(Qt::ControlModifier | Qt::Key_BracketRight));
    this->menu->addAction(tr("magnitude decrement"), this, SLOT(magnitudeDecr()), QKeySequence(Qt::ControlModifier | Qt::Key_BracketLeft));
    this->menu->addAction(tr("frequency increment"), this, SLOT(frequencyIncr()), QKeySequence(Qt::ControlModifier | Qt::Key_K));
    this->menu->addAction(tr("frequency decrement"), this, SLOT(frequencyDecr()), QKeySequence(Qt::ControlModifier | Qt::Key_L));
    this->menu->addAction(tr("initial animation"),     this, SLOT(initialAnimation()), QKeySequence(Qt::ControlModifier | Qt::Key_I));
    this->menu->addAction(tr("previous mode"), this, SLOT(formDecr()), QKeySequence(Qt::Key_L));
    this->menu->addAction(tr("next mode"),     this, SLOT(formIncr()), QKeySequence(Qt::Key_K));
    QAction* netAction = new QAction(tr("net"), this);
    this->connect(netAction, SIGNAL(triggered(bool)), SLOT(netTrigger()));
    netAction->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_N));
    netAction->setCheckable(true);
    netAction->setChecked(net);
    this->menu->addActions(QList<QAction*>() << netAction);
}

void GeometryWidget::initialAnimation() {
    initialPhase = currentPhase();
    initialTime = QTime::currentTime();
    animation->setFrequency(1.0f);
    animation->setMagnitude(1.0f);
}

void GeometryWidget::frequencyIncr()
{
    initialPhase = currentPhase();
    initialTime = QTime::currentTime();
    animation->multFrequency(1.2f);
}
void GeometryWidget::frequencyDecr()
{
    animation->multFrequency(0.8f);
}
void GeometryWidget::formIncr()
{
    if (data && data->modes().size() - 1 > form)
        setForm(form + 1);
    this->repaint();
}
void GeometryWidget::formDecr()
{
    if (data && form > 0)
        setForm(form - 1);
    this->repaint();
}

void GeometryWidget::setForm(int f) {
    form = f;
    this->timerEvent(0);
}

GeometryWidget::~GeometryWidget()
{
    animation->desertParent(this);
    if (!animation->isHaveParen())
        delete animation;
}

void GeometryWidget::paintCGL()
{
    repaintLoop->stop();
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, data->nodes().data());
    if (data->modes().size() > form && form >= 0) {
        glNormalPointer(GL_FLOAT, 0, data->form(form).data());
        glEnableClientState(GL_NORMAL_ARRAY);
    } else {
        glDisableClientState(GL_NORMAL_ARRAY);
        summator->setUniformValue("k", 0.0f);
    }
    data->render();
    if (net) {
        data->renderNet();
    }
    this->debugSpace(this->scene());

    if (data->modes().size() > form && form >= 0) {
        repaintLoop->start();
    }
}

void GeometryWidget::setModel(const GeometryForm* g)
{
    this->setScene(g->box());
    data = g;
    if (form >= data->modes().size()) {
        form = data->modes().size() - 1;
    }
    if (form < 0) {
        form = 0;
    }
}

void GeometryWidget::setModel(const GeometryForm& g)
{
    this->setModel(&g);
}

void GeometryWidget::initializeCGL()
{
    summator = new QOpenGLShaderProgram(this);
    if (!summator->addShaderFromSourceFile(QOpenGLShader::Vertex, ":codes/summator.vert")) {
        qWarning() << "compile impossible";
    }
    if (!summator->bind()) {
        qWarning() << "wasn't binded";
    }
    this->startTimer(10);
}

double GeometryWidget::currentPhase() const {
    return initialPhase + initialTime.msecsTo(QTime::currentTime()) / 1000. * acos(0.0) * 4. * animation->getFrequency();
}

void GeometryWidget::timerEvent(QTimerEvent*) {
    if (data && form >= 0 || form < data->modes().size()) {
        this->makeCurrent();
        double phase(currentPhase());
        summator->setUniformValue("k", static_cast<GLfloat>(sin(phase)) * animation->getMagnitude() * static_cast<GLfloat>(data->getDefoultMagnitude(form)));
    }
}
