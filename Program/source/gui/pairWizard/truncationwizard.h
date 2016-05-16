#ifndef TRUNCATIONWIZARD_H
#define TRUNCATIONWIZARD_H

#include <QDialog>

class QPushButton;
class FEMPair;
class QCheckBox;
class RelationDialog;
class ModelsPicker;
class FEMPairViewer;
class QSplitter;

class TruncationWizard : public QDialog
{
    Q_OBJECT

    TruncationWizard(QWidget* parent = 0);
    ~TruncationWizard();
public:
    static FEMPair* exec(QWidget* parent = 0);

public slots:
    void update();

private:
    QPushButton* const acceptB;
    FEMPair* result;
    QSplitter* const base;
    QWidget* const leftSide;
    QCheckBox* const align;
    QCheckBox* const scale;
    RelationDialog* const relations;
    ModelsPicker* const picker;
    FEMPairViewer* const viewer;
};

#endif // TRUNCATIONWIZARD_H
