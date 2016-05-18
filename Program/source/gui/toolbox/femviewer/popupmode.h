#ifndef POPUPMODE_H
#define POPUPMODE_H

#include <QFrame>

class FEMViewer;
class FEM;
class QSizeGrip;
class QGridLayout;

class PopupMode : public QFrame
{
    Q_OBJECT

    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);
public:
    PopupMode(QWidget* parent = 0);
    ~PopupMode();

    void setModel(const FEM* const);
    void setMode(int);
    QGridLayout* layout() const;

signals:
    void closed();
private:
    FEMViewer* const screen;
    QSizeGrip* const grip;
};

#endif // POPUPMODE_H
