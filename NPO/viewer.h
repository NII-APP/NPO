#ifndef VIEWER_H
#define VIEWER_H

#include <QSplitter>
#include <QModelIndex>
#include <QListView>
class GeometryWidget;

class Viewer : public QSplitter
{
    Q_OBJECT
    GeometryWidget* geometryWidget;
    QListView* geometriesView;
public:
    explicit Viewer(QWidget *parent = 0);
    ~Viewer();

signals:

public slots:
private slots:
    void listPatrol(QModelIndex);
};

#endif // VIEWER_H
