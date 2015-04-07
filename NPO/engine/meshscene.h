#ifndef MESHSCENE_H
#define MESHSCENE_H

#include <QWidget>
#include <QFrame>
#include <QSpinBox>
#include <QLabel>
#include "geometry.h"

class MeshPlace;

class MeshScene : public QWidget
{
    MeshPlace* const place;

    class Properties;
    Properties* const properties;

    void resizeEvent(QResizeEvent *);
public:
    explicit MeshScene(QWidget* parent = 0);
    ~MeshScene();

    void setData(const Mesh *model);
    void setData(const QVector<const Mesh*>& models);
    QVector<const Mesh*> getData() const;
public slots:
    void setMode(const int);
};

class MeshScene::Properties : public QFrame
{
public:
    QSpinBox* const form;
    QLabel* const formLabel;
    QLabel* const formSubLabel;
    Properties(QWidget* parent);
};

#endif // MESHSCENE_H
