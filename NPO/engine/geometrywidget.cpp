#include "geometrywidget.h"
#include "geometry.h"
#include <cassert>

GeometryWidget::GeometryWidget(QWidget *parent, QGLWidget *shareWidget, Qt::WindowFlags f)
    : CGLWidget(parent, shareWidget, f)
    , data(0)
    , repaintLoop(new QTimer(this))
    , animation(new AnimationOptions(this))
{
    paintDisable = [](GeometryWidget*){};
    initialPhase = 0.0f;
    initialTime = QTime::currentTime();
    form = 0;
    repaintLoop->setInterval(10);
    this->connect(repaintLoop, SIGNAL(timeout()), SLOT(repaint()));

    this->menu->addSeparator();

    animationAction = new QAction(tr("disable animation"), this);
    this->connect(animationAction, SIGNAL(triggered()), SLOT(triggerAnimation()));
    animationAction->setShortcut(QKeySequence(Qt::ControlModifier | Qt::ShiftModifier | Qt::Key_P));
    animationAction->setCheckable(true);
    animationAction->setChecked(false);
    this->menu->addAction(animationAction);

    pauseAction = new QAction(tr("pause animation"), this);
    this->connect(pauseAction, SIGNAL(triggered()), SLOT(triggerPause()));
    pauseAction->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_P));
    pauseAction->setCheckable(true);
    pauseAction->setChecked(false);
    this->menu->addAction(pauseAction);

    this->menu->addAction(tr("magnitude increment"), this, SLOT(magnitudeIncr()), QKeySequence(Qt::ControlModifier | Qt::Key_BracketRight));
    this->menu->addAction(tr("magnitude decrement"), this, SLOT(magnitudeDecr()), QKeySequence(Qt::ControlModifier | Qt::Key_BracketLeft));
    this->menu->addAction(tr("frequency increment"), this, SLOT(frequencyIncr()), QKeySequence(Qt::ControlModifier | Qt::Key_K));
    this->menu->addAction(tr("frequency decrement"), this, SLOT(frequencyDecr()), QKeySequence(Qt::ControlModifier | Qt::Key_L));
    this->menu->addAction(tr("initial animation"),   this, SLOT(initialAnimation()), QKeySequence(Qt::ControlModifier | Qt::Key_I));
    this->menu->addSeparator();
    this->menu->addAction(tr("previous mode"),       this, SLOT(formDecr()), QKeySequence(Qt::Key_L));
    this->menu->addAction(tr("next mode"),           this, SLOT(formIncr()), QKeySequence(Qt::Key_K));

    this->menu->addSeparator();
    netAction = new QAction(tr("mesh"), this);
    this->connect(netAction, SIGNAL(triggered()), SLOT(triggerNet()));
    netAction->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_N));
    netAction->setCheckable(true);
    netAction->setChecked(false);

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
    initialPhase = currentPhase();
    initialTime = QTime::currentTime();
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

void GeometryWidget::drawIdantityQuad() {
    glBegin(GL_QUADS);
    glColor3ub(0x77,0x77,0xDD);
    glVertex3d(-1.,-1., 0.9999999);
    glNormal3i(0,0,0);
    glVertex3d( 1.,-1., 0.9999999);
    glNormal3i(0,0,0);
    glColor3ub(0xAA,0xAA,0xAA);
    glVertex3d( 1., 1., 0.9999999);
    glNormal3i(0,0,0);
    glVertex3d(-1., 1., 0.9999999);
    glNormal3i(0,0,0);
    glEnd();
}

void GeometryWidget::setDisablePaintFunction(VoidFunction f) {
    paintDisable = f;
}

void GeometryWidget::paintCGL()
{
    repaintLoop->stop();

    if (!data) {
        paintDisable(this);
        return;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, data->nodes().data());
    if (isAnimation()) {
        glNormalPointer(GL_FLOAT, 0, data->form(form).data());
        glEnableClientState(GL_NORMAL_ARRAY);
    } else {
        glDisableClientState(GL_NORMAL_ARRAY);
        summator->setUniformValue("k", 0.0f);
    }
    data->render();
    if (netAction->isChecked()) {
        data->renderNet();
    }
    //this->debugSpace(this->scene());

    if (isAnimation()) {
        repaintLoop->start();
    }
}

void GeometryWidget::setModel(const GeometryForm* g)
{
    this->setScene(g->box());
    data = g;
    if (form >= data->modes().size()) {
        form = static_cast<int>(data->modes().size() - 1);
    }
    if (form < 0) {
        form = 0;
    }
    this->repaint();
}

void GeometryWidget::setModel(const GeometryForm& g)
{
    this->setModel(&g);
}

void GeometryWidget::initializeCGL()
{
    summator = new QOpenGLShaderProgram(this);
    Q_ASSERT(summator->addShaderFromSourceFile(QOpenGLShader::Vertex, ":codes/summator.vert"));
    Q_ASSERT(summator->bind());
    this->startTimer(10);
}

void GeometryWidget::triggerAnimation() {
    QList<QAction*> actions(this->menu->actions());
    int i(0);
    while (actions.at(i) != pauseAction) {
        ++i;
    }
    while (!actions.at(++i)->text().isEmpty()) {
        actions[i]->setDisabled(animationAction->isChecked());
    }
    this->repaint();
}

double GeometryWidget::currentPhase() const {
    return initialPhase + (initialTime.msecsTo(QTime::currentTime()) + pauseTime) / 1000. * acos(0.0) * 4. * animation->getFrequency();
}

void GeometryWidget::timerEvent(QTimerEvent*) {
    if (isAnimation()) {
        if (pauseAction->isChecked()) {
            initialTime = QTime::currentTime();
        }
        this->makeCurrent();
        double phase(currentPhase());
        summator->setUniformValue("k", static_cast<GLfloat>(sin(phase)) * animation->getMagnitude() * static_cast<GLfloat>(data->getDefoultMagnitude(form)));
    }
}

void GeometryWidget::triggerPause() {
    pauseTime = initialTime.msecsTo(QTime::currentTime());
}
