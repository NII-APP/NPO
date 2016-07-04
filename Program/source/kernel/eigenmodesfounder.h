#ifndef EigenModesFounder_H
#define EigenModesFounder_H

#include <QObject>
#include <QThread>

#include "igofesolver.h"

class FEM;
class QString;
class QLabel;
class CMatrix;
class EigenModes;

/**
 * @brief The EigenModesFounder class allow to find eigen modes in one of three ways: readModes from file,
 * calculateModes with kiselev solver and identificateModes with identification wizard
 */
class EigenModesFounder : public QObject
{
    Q_OBJECT

public:
    EigenModesFounder(FEM * const, QObject * const parent);
    ~EigenModesFounder();

    FEM* model() const { return fem; }

    /**
     * @brief identificateModes start the modes identification wizard and return the control imidietly.
     * When the wizard had finish the work and modes ready to import, the founder emit the modesMined.
     * Then founder save modes to model and emit modesReceived. Hereupon dounder emit the finished signal.
     * If the widget doesn't take modes, object just emit finished
     */
    void identificateModes();
    /**
     * @brief readModes imidietli emit modesMined and start the FEM::read with given name and start the
     * loop timer to catch te moment, when the modes is occur, founder emit rhe modesReceived and start
     * to track the mac to holt it's modification. when the FEM::read return the control to thread,
     * object emit MACUpdated (to know positively that slots informed about final mac state) and finished
     * @param file name with modes (unv or bdf/dat)
     */
    void readModes(const QString& filename);
    /**
     * @brief calculateModes start the IgoFESolver ant start to track the calculation process. Founder
     * emit modesMined when solver have take the information aboutmodes and modesReceived  when the
     * eigen modes saved in model
     * @param solver options
     */
    void calculateModes(const IgoFESolver::SolverOptions& options);

signals:
    void modesMined();
    void modesReceived();
    void MACUpdated();
    void finished();

private:
    FEM* const fem;
};

#endif // EigenModesFounder_H
