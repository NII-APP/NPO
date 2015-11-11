#ifndef FILENAMEEDIT_H
#define FILENAMEEDIT_H

#include <QLineEdit>
#include <functional>

class QPushButton;
class QFileDialog;

class FileNameEdit : public QLineEdit {
    Q_OBJECT

private:
    void keyPressEvent(QKeyEvent *);
    void resizeEvent(QResizeEvent *);
    bool eventFilter(QObject *, QEvent *);
    void valueTest();

public:
    FileNameEdit(QWidget* parent = 0);
    void setOpenFileFunction(std::function<QString(QWidget*)> f) {
        __onClick = f;
    }

signals:
    void fileNameChanged(QString);
private slots:
    void callDialod();

private:
    QPushButton* const __pb;
    QWidget* const __warning;
    std::function<QString(QWidget*)> __onClick;
};


#endif // FILENAMEEDIT_H
