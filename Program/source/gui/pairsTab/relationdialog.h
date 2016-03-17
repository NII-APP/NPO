#ifndef RELATIONDIALOG_H
#define RELATIONDIALOG_H

#include <QDialog>
#include <QEventLoop>
#include <QtAlgorithms>
#include "fempair.h"
#include "gui/viewerTab/viewertab.h"
#include <QFrame>
#include <QLabel>
#include <QPushButton>

class RelationDialog : public QWidget
{
    Q_OBJECT
    const QImage toggleOn;
    const QImage toggleOff;

    FEMPair* pair;
    FEMPair::Relation& relation() const { return pair->relations(); }

    typedef QVector<QLabel*> Labels;
    Labels  leftL;
    Labels  rightL;
    int maxW;

    ViewerTab* leftF;
    ViewerTab* rightF;
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

    void buildLabels(Labels&, const FEM &);
    void renderLabels();

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

    bool eventFilter(QObject *, QEvent *);

    void bgUpdate();
    void popupConfig(QFrame *&, ViewerTab *&, const FEM &v);

    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

    void updateLines();

public:
    RelationDialog(FEMPair* forEdit, QWidget *parent = 0);
    void setPair(FEMPair* p);
    static void run(FEMPair *forEdit, QWidget* parent = 0);
    
signals:
    void updateMac(const FEMPair::Relation&);

public slots:

    void showForm(QObject *w);
    
};

#endif // RELATIONDIALOG_H
