#ifndef RELATIONDIALOG_H
#define RELATIONDIALOG_H

#include <QDialog>
#include <QEventLoop>
#include <QtAlgorithms>
#include "geometrypair.h"
#include "geometrywidget.h"
#include <QFrame>
#include <QLabel>

class RelationDialog : public QDialog
{
    Q_OBJECT
    const QImage toggleOn;
    const QImage toggleOff;

    GeometryPair& pair;
    GeometryPair::Relation& relation;

    typedef QVector<QLabel*> Labels;
    Labels  leftL;
    Labels  rightL;
    int maxW;

    GeometryWidget* leftF;
    GeometryWidget* rightF;
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

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

    bool eventFilter(QObject *, QEvent *);

    void bgUpdate();
    void popupConfig(QFrame *&, GeometryWidget *&, const GeometryForm &v);

    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

    void updateLines();

public:
    RelationDialog(GeometryPair* forEdit, QWidget *parent = 0);

    static void run(GeometryPair* forEdit, QWidget* parent = 0);
    
signals:
    
public slots:

    void showForm(QObject *w);
    
};

#endif // RELATIONDIALOG_H
