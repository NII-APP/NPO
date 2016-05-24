#include "relationdialog.h"

#include <cassert>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QDir>

#include "application.h"
#include "identity.h"
#include "gui/toolbox/femviewer/popupmode.h"


RelationDialog::RelationDialog(QWidget *parent)
  : QWidget(parent)
  , toggleOff(":/media/resource/images/togleOff.png")
  , toggleOn(":/media/resource/images/togleOn.png")
  , pair(nullptr)
  , leftL(0)
  , rightL(0)
  , maxW(0)
  , leftP(new PopupMode)
  , rightP(new PopupMode)
  , title1(new QLabel(this))
  , title2(new QLabel(this))
  , underToggle(-1)
  , underLeftToggle(false)
  , lineingState(-1)
  , lineingLeft(false)
{
    this->setContentsMargins(10, 0, 10, 0);
}

void RelationDialog::setPair(FEMPair *p)
{
    pair = p;

    qDebug() << "set pair" << p;

    if (!pair) {
        qDeleteAll(leftL);
        qDeleteAll(rightL);
        return;
    }

    buildLabels(leftL, *pair->a());
    buildLabels(rightL, *pair->b());

    foreach (QLabel* l, leftL)
        l->resize(maxW, l->height());
    foreach (QLabel* l, rightL)
        l->resize(maxW, l->height());

    this->setMinimumSize(maxW * 2 + 200, leftL.isEmpty() ? 0 : (leftL.first()->height() * (leftL.size() > rightL.size() ? leftL.size() : rightL.size())));
    const QSize minSize(std::max(this->minimumWidth(), this->width()), std::max(this->minimumHeight(), this->height()));
    if (minSize != this->size()) {
        this->resize(minSize);
    }

    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(0x00,0x00,0xFF));
    this->setPalette(palette);
    this->setMouseTracking(true);

    scarfUpRelations();
}

CIndexes RelationDialog::inverseRelations(const CIndexes& v, int newSize)
{
#ifndef QT_NO_DEBUG
    assert(*std::max_element(v.begin(), v.end()) <= newSize);
#endif
    CIndexes r(newSize, -1);
    for (size_t i(0); i != v.size(); ++i) if (v[i] >= 0) {
        r[v[i]] = static_cast<int>(i);
    }
    return r;
}

CIndexes RelationDialog::belchRelations() const
{
    if (pair == nullptr) {
        return CIndexes();
    }
    if (pair->a() != pair->theory()) {
        return inverseRelations(relation, static_cast<int>(pair->theory()->getModes().size()));
    } else {
        return relation;
    }
}

void RelationDialog::scarfUpRelations()
{
    if (pair == nullptr) {
        return;
    }
    if (pair->a() != pair->theory()) {
        relation = inverseRelations(pair->relations(), static_cast<int>(pair->a()->getModes().size()));
    } else {
        relation = pair->relations();
    }
    bgUpdate();
    updateLines();
    emitRelationsUpdated();
}

RelationDialog::~RelationDialog()
{
    leftP->deleteLater();
    rightP->deleteLater();
}

void RelationDialog::bgUpdate()
{
    for (int i(0); i != rightL.size(); ++i) {
        rightL[i]->setBackgroundRole(QPalette::Dark);
    }
    for (int i(0); i != leftL.size(); ++i) {
        if (relation[i] >= 0) {
            rightL[relation[i]]->setBackgroundRole(QPalette::Light);
            leftL[i]->setBackgroundRole(QPalette::Light);
        } else {
            leftL[i]->setBackgroundRole(QPalette::Dark);
        }
    }
}

void RelationDialog::buildLabels(Labels &lbls, const FEM& g)
{
    qDeleteAll(lbls);
    lbls.resize(static_cast<int>(g.getModes().size()));

    QPalette p(this->palette());
    p.setBrush(QPalette::Light, QColor(0xFF,0xFF,0xFF));
    p.setBrush(QPalette::Dark, QColor(0x88,0x88,0x88));

    for (int i = 0; i != lbls.size(); ++i) {
        QLabel* l(new QLabel(Application::identity()->tr("form selector label").arg(QString::number(i + 1), QString::number(g.getModes().at(i).frequency())),this));
        l->resize(l->sizeHint());
        l->setFrameStyle(QFrame::Plain | QFrame::StyledPanel);
        l->setLineWidth(1);
        l->setCursor(QCursor(Qt::PointingHandCursor));
        l->setContextMenuPolicy(Qt::CustomContextMenu);
        l->installEventFilter(this);
        l->setPalette(p);
        l->setAutoFillBackground(true);
        lbls[i] = l;
        if (maxW < l->sizeHint().width())
            maxW = l->sizeHint().width();
    }

    leftP->resize(maxW, maxW);
    rightP->resize(maxW, maxW);

    renderLabels();
}

void RelationDialog::renderLabels()
{
    leftCapacity = (this->height() - 20) * 1.0f / leftL.size();
    rightCapacity = (this->height() - 20) * 1.0f / rightL.size();
    float myH(leftL.isEmpty() ? 0 : leftL.first()->height() / 2.);
    for (int i(0); i != leftL.size(); ++i)
    {
        leftL[i]->move(this->contentsMargins().left(), leftCapacity * (i + .5f) - myH + 20);
        leftL[i]->show();
    }
    for (int i(0); i != rightL.size(); ++i)
    {
        rightL[i]->move(this->width() - this->contentsMargins().right() - maxW, rightCapacity * (i + .5f) - myH + 20);
        rightL[i]->show();
    }

    this->update();
}

bool RelationDialog::eventFilter(QObject* o, QEvent* e)
{
    if (!o || e->type() != QEvent::MouseButtonPress)
        return false;
    QMouseEvent* me(static_cast<QMouseEvent*>(e));
    if (me->button() == Qt::LeftButton) {
        showForm(o);
    }
    return true;
}
void RelationDialog::showForm(QObject* s)
{
    QWidget* wgt(0);
    int i, d;
    for (i = 0; i != leftL.size(); ++i) if (leftL[i] == s) {
        wgt = leftL[d = i];
    }
    if (wgt) {
        leftP->move(this->mapToGlobal(QPoint(0,0)) + wgt->geometry().bottomLeft());
        leftP->setModel(pair->a());
        leftP->setMode(d);
        leftP->show();
        return;
    }
    for (i = 0; i != rightL.size(); ++i) if (rightL[i] == s) {
        wgt = rightL[d = i];
    }
    if (wgt) {
        rightP->move(this->mapToGlobal(QPoint(0,0)) + wgt->geometry().bottomLeft());
        rightP->setModel(pair->b());
        rightP->setMode(d);
        rightP->show();
        return;
    }
}

void RelationDialog::resizeEvent(QResizeEvent*)
{
    title1->move(this->contentsMargins().left(),5);
    title2->move(this->width() - this->contentsMargins().right() - maxW, 5);
    renderLabels();
}


void RelationDialog::paintEvent(QPaintEvent *)
{
    if (!pair) {
        return;
    }
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    p.setRenderHint(QPainter::Antialiasing);
    QPen pen(p.pen());
    pen.setStyle(Qt::DashDotDotLine);
    pen.setWidth(3);
    pen.setColor(QColor(0x82,0x87,0x90));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    p.setPen(pen);
    //p.fillRect(this->rect(), QColor(rand() % 0x100, rand() % 0x100, rand() % 0x100));
    p.fillRect(this->rect(), QColor(0xF0, 0xF0, 0xF0));
    for (int i = 0; i != relation.size(); ++i)
    {
        if (relation[i] >= 0)
        {
            p.drawLine(this->contentsMargins().left() + maxW + 24, leftCapacity * (i + .5f) + 20,
                       this->width() - this->contentsMargins().right() - maxW - 24, rightCapacity * (relation[i] + .5f) + 20);
        }
    }
    pen.setColor(QColor(0x88,0xFF, 0x88));
    p.setPen(pen);
    if (lineingState >= 0)
    {
        p.drawLine(source, tracking);
    }
    pen.setColor(QColor(0x82,0x87,0x90));
    p.setPen(pen);
    for (int i = 0; i != leftL.size(); ++i) {
        p.drawImage(this->contentsMargins().left() + maxW, leftCapacity * (i + .5f) - toggleOff.height() / 2 + 20,
                    (underToggle == i && underLeftToggle) ||
                    (lineingState == i && lineingLeft) ? toggleOn : toggleOff);
    }
    p.rotate(180);
    p.translate(-this->width(),-this->height());
    for (int i(rightL.size()); i; )
    {
        --i;
        p.drawImage(this->contentsMargins().right() + maxW, rightCapacity * (i + .5f) - toggleOff.height() / 2 - 0,
                    (underToggle == rightL.size() - i - 1 && !underLeftToggle) ||
                    (lineingState == rightL.size() - i - 1 && !lineingLeft) ? toggleOn :  toggleOff);
    }
    p.rotate(180);
    p.translate(-this->width(),-this->height());
}

void RelationDialog::mouseMoveEvent(QMouseEvent *e)
{
    tracking = e->pos();

    const QPoint& p(e->pos());
    QRectF crnt(QPointF(this->contentsMargins().left() + maxW, (leftCapacity - toggleOff.height()) / 2 + 20), toggleOff.size());
    int i = -1;
    bool side(false);
    while (++i != leftL.size() && !crnt.contains(p))
    {
        crnt.translate(0, leftCapacity);
    }
    if (i == leftL.size())
    {
        crnt.moveTo(this->width() - maxW - this->contentsMargins().right() - toggleOff.width(),(rightCapacity - toggleOff.height()) / 2 + 20);
        i = -1;
        while (++i != rightL.size() && !crnt.contains(p))
        {
            crnt.translate(0, rightCapacity);
        }
        if (i == rightL.size())
        {
            i = -1;
        }
    }
    else
    {
        side = true;
    }
    if (lineingState >= 0 || i != underToggle || (underLeftToggle != side && i != -1))
    {
        this->setCursor(QCursor(i >= 0 ? Qt::PointingHandCursor : Qt::ArrowCursor));
        underToggle = i;
        underLeftToggle = side;
        updateLines();
    }
}

void RelationDialog::updateLines()
{
    this->update(
        QRect(QPoint(leftL.first()->geometry().right(), (leftCapacity > rightCapacity ? rightL : leftL).first()->geometry().top()),
              QPoint(rightL.last()->geometry().left(), (leftCapacity > rightCapacity ? rightL : leftL).last()->geometry().bottom())));
}

void RelationDialog::emitRelationsUpdated()
{
    pair->setRelations(belchRelations());
    emit relationsUpdated(pair->relations());
    emit relationsModified(relations());
}

void RelationDialog::mousePressEvent(QMouseEvent *)
{
    if (underToggle >= 0 && lineingState < 0)
    {
        lineingState = underToggle;
        lineingLeft = underLeftToggle;
        if (underLeftToggle) {
            relation[underToggle] = -1;
            source = QPoint(this->contentsMargins().left() + maxW + 24, leftCapacity * (underToggle + .5f) + 20);
        } else {
            CIndexes::iterator elem(qFind(relation.begin(), relation.end(), underToggle));
            if (elem != relation.end())
                *elem = -1;
            source = QPoint(this->width() - this->contentsMargins().right() - maxW - 24, rightCapacity * (underToggle + .5f) + 20);
        }
        emitRelationsUpdated();
    }
    else if (underToggle >= 0 && underLeftToggle != lineingLeft)
    {
        if (underLeftToggle)
        {
            relation[underToggle] = lineingState;
        }
        else
        {
            CIndexes::iterator esc(qFind(relation.begin(), relation.end(), underToggle));
            if (esc != relation.end())
                *esc = -1;
            relation[lineingState] = underToggle;
        }
        lineingState = -1;

        emitRelationsUpdated();
    }
    else if (lineingState >= 0)
    {
        lineingState = -1;
        emitRelationsUpdated();
    }

    bgUpdate();
    updateLines();
}

void RelationDialog::run(FEMPair* forEdit, QWidget* parent)
{
    QEventLoop* l(new QEventLoop(parent));
    RelationDialog* dialog(new RelationDialog(parent));
    dialog->setPair(forEdit);
    l->connect(dialog, SIGNAL(finished(int)), SLOT(quit()));
    dialog->show();
    l->exec();
}
