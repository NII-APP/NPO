#ifndef TRUNCATIONWIZARD_H
#define TRUNCATIONWIZARD_H

#include <QDialog>
#include "engine/geometrypair.h"

class TruncationWizard : public QDialog
{
public:
    TruncationWizard(QWidget* parent = 0);
    ~TruncationWizard();

    static GeometryPair* exec(QWidget* parent);
};

#endif // TRUNCATIONWIZARD_H
