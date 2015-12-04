#ifndef FILENAMEEDIT_H
#define FILENAMEEDIT_H

#include <QLineEdit>
#include <tr1/functional>

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
    void setOpenFileFunction(std::tr1::function<QString(FileNameEdit*)> f) {
        __onClick = f;
    }

signals:
    void fileNameChanged(QString);
private slots:
    void callDialod();

private:
    QPushButton* const __pb;
    QWidget* const __warning;
    std::tr1::function<QString(FileNameEdit*)> __onClick;
};


#endif // FILENAMEEDIT_H
