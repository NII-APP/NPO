#ifndef TABLISTWIDGET_H
#define TABLISTWIDGET_H

#include <QWidget>
#include <QMap>

class QPushButton;
class QString;
class QIcon;
class QVBoxLayout;
class QLayoutItem;

class TabListWidget : public QWidget
{
    Q_OBJECT
    typedef QMap<QPushButton*, QWidget*> Items;

    QVBoxLayout* layout();
    int addTab(Items::iterator item);
    Items::iterator current();
    int toId(Items::iterator);

public:
    TabListWidget(QWidget *parent = 0);
    ~TabListWidget();

    int	addTab(QWidget* page, const QString& label);
    int	addTab(QWidget* page, const QIcon& icon, const QString& label);
    void disable(int id, bool disabled = true);
    int currentId();

private slots:
    void togleTab();

private:
    Items items;
    QLayoutItem* stretch;
};

#endif // TABLISTWIDGET_H
