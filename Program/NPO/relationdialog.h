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

    MeshPair* pair;
    MeshPair::Relation& relation() const { return pair->relations(); }

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

    void bildLabels(Labels&, FEM &);
    void renderLabels();

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

    bool eventFilter(QObject *, QEvent *);

    void bgUpdate();
    void popupConfig(QFrame *&, Viewer *&, const FEM &v);

    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

    void updateLines();

public:
    RelationDialog(MeshPair* forEdit, QWidget *parent = 0);
    void setPair(MeshPair* p);
    static void run(MeshPair* forEdit, QWidget* parent = 0);
    
signals:
    void updateMac(const MeshPair::Relation&);

public slots:

    void showForm(QObject *w);
    
};

#endif // RELATIONDIALOG_H
