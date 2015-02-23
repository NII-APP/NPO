#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSplitter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    //from where project was be loaded or saved at last time
    QString disposed;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
    void save();
    void saveAs();
    void open();
};

#endif // MAINWINDOW_H
