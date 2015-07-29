#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTabBar;
class QSplitter;
class QTabWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT

    //from where project was be loaded or saved in last time
    QString disposed;

    QWidget* status;

    class TabWidget : public QTabWidget {
    public:
        TabWidget(QWidget* parent) : QTabWidget(parent) {}
        void setTabBar(QTabBar* b) { QTabWidget::setTabBar(b); }
    };

    void load(const QString& location);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void porjectLoaded();

public slots:
    void save();
    void saveAs();
    void open();
    void statusPush(QWidget*);
    void statusPush(const QString &);
    void statusInsertBefore(QWidget* which, QWidget* before);
    void statusClear();
};

#endif // MAINWINDOW_H
