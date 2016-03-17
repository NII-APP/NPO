#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTabBar;
class QSplitter;
class QTabWidget;
class ViewerTab;
class PairsTab;
class MainWindow : public QMainWindow
{
    Q_OBJECT

    ViewerTab* const __modelsGUI;
    PairsTab* const __pairsGUI;

    class TabWidget : public QTabWidget {
    public:
        TabWidget(QWidget* parent) : QTabWidget(parent) {}
        void setTabBar(QTabBar* b) { QTabWidget::setTabBar(b); }
    };

    void load(const QString& location);
    void saveProjectLocation(const QString& v) const;
    QString projectLocation() const;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void projectLoaded();

public slots:
    void save();
    void saveAs();
    void open();
    void closePorject();
};

#endif // MAINWINDOW_H
