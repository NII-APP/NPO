#ifndef MODESIDENTIFICATIONWIZARD_H
#define MODESIDENTIFICATIONWIZARD_H

#include <QDialog>
#include <QMap>
#include "tablistwidget.h"
#include "modesidentresult.h"

class FEM;
class QString;
class QPushButton;
class FEMViewer;
class C2dChart;
class QSplitter;
class AFRArray;
class QTableWidget;
class CSlider;
class QTextBrowser;
class ModesIdentChart;

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

class ModesIdentificationWizard::MethodSelector : public TabListWidget {
    Q_OBJECT
public:
    explicit ModesIdentificationWizard::MethodSelector(QWidget* parent);
    ~MethodSelector();
public slots:
    void updateResultsCurrent(ModesIdentResult);
private:
    QMap<int, QTextBrowser*> __resultDisplays;
};

class ModesIdentificationWizard::ManualController : public QWidget {
    Q_OBJECT
public:
    explicit ModesIdentificationWizard::ManualController(const FEM * const model, QWidget* parent);
    ~ManualController();
public slots:
    void setAFR(QString);
    void setModeFrequency(double);
private slots:
    void changeSplitterOrientation();
private:
    QSplitter* const __splitter;
    FEMViewer* const __viewer;
    ModesIdentChart* const __chart;
    AFRArray* __afr;
};

#endif // MODESIDENTIFICATIONWIZARD_H
