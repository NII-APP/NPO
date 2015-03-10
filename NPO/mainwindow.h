#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTabBar;
class QSplitter;
class QTabWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    //from where project was be loaded or saved at last time
    QString disposed;

    class TabWidget : public QTabWidget {
    public:
        TabWidget(QWidget* parent) : QTabWidget(parent) {}
        void setTabBar(QTabBar* b) { QTabWidget::setTabBar(b); }
    };

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void porjectLoaded();

public slots:
    void save();
    void saveAs();
    void open();
};

#endif // MAINWINDOW_H
