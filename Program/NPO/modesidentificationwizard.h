#ifndef MODESIDENTIFICATIONWIZARD_H
#define MODESIDENTIFICATIONWIZARD_H

#include <QDialog>
#include <QLineEdit>

class FEM;
class QString;
class QPushButton;
class FEMViewer;
class C2dChart;
class QSplitter;
class AFRArray;
class QTableWidget;

class ModesIdentificationWizard : public QDialog
{
    Q_OBJECT

    class MethodSelector;
    class ManualController;

    void stylize();

    void accept();
    void reject();
public:
    ModesIdentificationWizard(const FEM* who, QWidget* parent = 0);
    ~ModesIdentificationWizard();

    static void identifyModes(const FEM* who, QWidget* parent = 0);

private:
    MethodSelector* const __method;
    ManualController* const __controller;
    const FEM* const __model;
};

class ModesIdentificationWizard::MethodSelector : public QWidget {
    class Signboard;
public:
    explicit ModesIdentificationWizard::MethodSelector(QWidget* parent);
private:
    QTableWidget* const __resultsTable;
    QList<Signboard*> __methods;
};

class ModesIdentificationWizard::MethodSelector::Signboard : public QWidget {
public:
    explicit ModesIdentificationWizard::MethodSelector::Signboard(QWidget* parent);

    void setTitle(const QString&);
    QWidget* getBoard();
private:
    QWidget* __board;
    QPushButton* __title;
};

class ModesIdentificationWizard::ManualController : public QWidget {
    Q_OBJECT
private:
    class FileInput;
public:
    explicit ModesIdentificationWizard::ManualController(const FEM * const model, QWidget* parent);
public slots:
    void setAFR(QString);
private slots:
    void changeSplitterOrientation();
private:
    QSplitter* const __splitter;
    FEMViewer* const __viewer;
    C2dChart* const __chart;
    AFRArray* __afr;
};

class ModesIdentificationWizard::ManualController::FileInput : public QLineEdit {
    Q_OBJECT
private:
    void keyPressEvent(QKeyEvent *);
    void resizeEvent(QResizeEvent *);
public:
    ModesIdentificationWizard::ManualController::FileInput(QWidget* parent = 0);
signals:
    void fileNameChanged(QString);
private slots:
    void callDialod();

private:
    QPushButton* const __pb;
};

#endif // MODESIDENTIFICATIONWIZARD_H
