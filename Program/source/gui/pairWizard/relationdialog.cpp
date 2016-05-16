#include "relationdialog.h"
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QDir>
#include "application.h"
#include "identity.h"

RelationDialog::RelationDialog(QWidget *parent)
  : QWidget(parent)
  , toggleOff(":/media/resource/images/togleOff.png")
  , toggleOn(":/media/resource/images/togleOn.png")
  , pair(nullptr)
  , leftL(0)
  , rightL(0)
  , maxW(0)
  , underToggle(-1)
  , underLeftToggle(false)
  , lineingState(-1)
  , lineingLeft(false)
  , title1(new QLabel(this))
  , title2(new QLabel(this))
{
    this->setContentsMargins(10, 0, 10, 0);
}

void RelationDialog::setPair(FEMPair *p) {
    pair = p;

    qDebug() << "set pair" << p;

    if (!pair) {
        qDeleteAll(leftL);
        qDeleteAll(rightL);
        return;
    }

    foreach (QLabel* l, leftL) {
        l->resize(maxW, l->height());
    }
    foreach (QLabel* l, rightL) {
        l->resize(maxW, l->height());
    }

    //this->setMinimumSize(maxW * 2 + 200, leftL.first()->height() * (leftL.size() > rightL.size() ? leftL.size() : rightL.size()));
    //this->resize(this->minimumWidth(), this->minimumHeight());
    
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(0x00,0x00,0xFF));
    this->setPalette(palette);
    this->setMouseTracking(true);

    if (relation().size() != leftL.size()) {
        relation().resize(leftL.size());
        for (int i(0); i < static_cast<int>(relation().size()); ++i) {
            relation()[i] = rightL.size() > i ? i : -1;
        }
    }
    bgUpdate();
}

void RelationDialog::bgUpdate()
{
    for (int i(0); i != rightL.size(); ++i) {
        rightL[i]->setBackgroundRole(QPalette::Dark);
    }
    for (int i(0); i != leftL.size(); ++i) {
        if (relation()[i] >= 0) {
            rightL[relation()[i]]->setBackgroundRole(QPalette::Light);
            leftL[i]->setBackgroundRole(QPalette::Light);
        } else {
            leftL[i]->setBackgroundRole(QPalette::Dark);
        }
    }
}

void RelationDialog::buildLabels(Labels &lbls, const FEM& g)
{
    qDebug() << "built labels";
    qDeleteAll(lbls);
    lbls.resize(static_cast<int>(g.getModes().size()));
    qDebug() << "\t " << lbls.size();

    QPalette p(this->palette());
    p.setBrush(QPalette::Light, QColor(0xFF,0xFF,0xFF));
    p.setBrush(QPalette::Dark, QColor(0x88,0x88,0x88));

    for (int i = 0; i != lbls.size(); ++i) {
        qDebug() << "label it" << i;
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
    if (me->button() == Qt::LeftButton)
        showForm(o);
    return true;
}
void RelationDialog::showForm(QObject* s)
{
    QWidget* wgt(0);
    int i, d;
    for (i = 0; i != leftL.size(); ++i)
    {
        if (leftL[i] == s)
            wgt = leftL[d = i];
    }
    if (wgt)
    {
        leftP->move(this->mapToGlobal(QPoint(0,0)) + wgt->geometry().bottomLeft() - QPoint(0,
                                   (wgt->height() + leftP->height()) * (d - 1 > leftL.size() / 2)));
        leftF->setMode(d);
        leftP->show();
        return;
    }
    for (i = 0; i != rightL.size(); ++i)
    {
        if (rightL[i] == s)
            wgt = rightL[d = i];
    }
    if (wgt)
    {
        rightP->move(this->mapToGlobal(QPoint(0,0)) + wgt->geometry().bottomLeft() - QPoint(0,
                                   (wgt->height() + rightP->height()) * (d - 1 > rightL.size() / 2)));
        rightF->setMode(d);
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
    for (int i = 0; i != relation().size(); ++i)
    {
        if (relation()[i] >= 0)
        {
            p.drawLine(this->contentsMargins().left() + maxW + 24, leftCapacity * (i + .5f) + 20,
                       this->width() - this->contentsMargins().right() - maxW - 24, rightCapacity * (relation()[i] + .5f) + 20);
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

void RelationDialog::mousePressEvent(QMouseEvent *)
{
    if (pair == nullptr) {
        return;
    }
    if (underToggle >= 0 && lineingState < 0)
    {
        lineingState = underToggle;
        lineingLeft = underLeftToggle;
        if (underLeftToggle) {
            relation()[underToggle] = -1;
            source = QPoint(this->contentsMargins().left() + maxW + 24, leftCapacity * (underToggle + .5f) + 20);
        } else {
            FEMPair::Relation::iterator elem(qFind(relation().begin(), relation().end(), underToggle));
            if (elem != relation().end())
                *elem = -1;
            source = QPoint(this->width() - this->contentsMargins().right() - maxW - 24, rightCapacity * (underToggle + .5f) + 20);
        }
        emit updateMac(relation());
    }
    else if (underToggle >= 0 && underLeftToggle != lineingLeft)
    {
        if (underLeftToggle)
        {
            relation()[underToggle] = lineingState;
        }
        else
        {
            FEMPair::Relation::iterator esc(qFind(relation().begin(), relation().end(), underToggle));
            if (esc != relation().end())
                *esc = -1;
            relation()[lineingState] = underToggle;
        }
        lineingState = -1;

        emit updateMac(relation());
    }
    else if (lineingState >= 0)
    {
        lineingState = -1;
        emit updateMac(relation());
    }

    bgUpdate();
    updateLines();
}
