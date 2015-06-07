#ifndef RELATIONDIALOG_H
#define RELATIONDIALOG_H

#include <QDialog>
#include <QEventLoop>
#include <QtAlgorithms>
#include "engine/geometrypair.h"
#include "viewer.h"
#include <QFrame>
#include <QLabel>
#include <QPushButton>

class RelationDialog : public QWidget
{
    Q_OBJECT
    const QImage toggleOn;
    const QImage toggleOff;

    GeometryPair* pair;
    GeometryPair::Relation& relation() const { return pair->relations(); }

    typedef QVector<QLabel*> Labels;
    Labels  leftL;
    Labels  rightL;
    int maxW;

    Viewer* leftF;
    Viewer* rightF;
    QFrame* leftP;
    QFrame* rightP;
    QLabel* title1;
    QLabel* title2;

    float leftCapacity;
    float rightCapacity;

    int underToggle;
    bool underLeftToggle;
    int lineingState;
    bool lineingLeft;
    QPoint source;
    QPoint tracking;

    void bildLabels(Labels&, GeometryForm &);
    void renderLabels();

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

    bool eventFilter(QObject *, QEvent *);

    void bgUpdate();
    void popupConfig(QFrame *&, Viewer *&, const GeometryForm &v);

    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

    void updateLines();

public:
    RelationDialog(GeometryPair* forEdit, QWidget *parent = 0);
    void setPair(GeometryPair* p);
    static void run(GeometryPair* forEdit, QWidget* parent = 0);
    
signals:
    void updateMac(const GeometryPair::Relation&);

public slots:

    void showForm(QObject *w);
    
};

#endif // RELATIONDIALOG_H
