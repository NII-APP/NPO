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
#include "cindexes.h"

class PopupMode;

class RelationDialog : public QWidget
{
    Q_OBJECT
    const QImage toggleOn;
    const QImage toggleOff;

    FEMPair* pair;

    typedef QVector<QLabel*> Labels;
    Labels leftL;
    Labels rightL;
    int maxW;

    PopupMode* const leftP;
    PopupMode* const rightP;
    QLabel* const title1;
    QLabel* const title2;

    float leftCapacity;
    float rightCapacity;

    int underToggle;
    bool underLeftToggle;
    int lineingState;
    bool lineingLeft;
    QPoint source;
    QPoint tracking;

    CIndexes relation;

    void buildLabels(Labels&, const FEM &);
    void renderLabels();

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

    bool eventFilter(QObject *, QEvent *);

    void bgUpdate();

    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

    void updateLines();

    void emitRelationsUpdated();
public:
    RelationDialog(QWidget *parent = 0);
    ~RelationDialog();
    void setPair(FEMPair* p);
    static void run(FEMPair *forEdit, QWidget* parent = 0);

    const CIndexes& relations() const { return relation; }
signals:
    void relationsChanged();

public slots:
    void scarfUpRelations();
    void showForm(QObject *w);

};

#endif // RELATIONDIALOG_H
