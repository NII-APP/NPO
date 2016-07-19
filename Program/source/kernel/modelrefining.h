#ifndef ModelRefining_H
#define ModelRefining_H

#include "fem.h"
#include "fempair.h"
#include "igofesolver.h"
#include <functional>
#include "crange.h"

typedef std::function<double(const CVector&, const CVector&, const CVector&)> ScaleFactor;
typedef std::vector<CRealRange> RangeVector;
typedef std::vector<bool> BoolVector;

class ModelRefining
{

public:
    ModelRefining (const FEM *);
    ~ModelRefining ();

    void updateFreqByElasticity(const CVector& testFreq);

    struct StopingCondition {
        StopingCondition();

        double accuracy;
        int stepCount;
        double stognation;
        int stognationLength;

        bool isAccuracyCondition;
        bool isStepCountCondition;
        bool isStognationCondition;

        virtual bool stop(const CVector&);
    };

    void setScaleFactor(const ScaleFactor& scaleFactorIn);
    const ScaleFactor& getScaleFactor();

    void setChangeOfElement(const BoolVector&);
    const BoolVector& getChangeOfElement();

    void setVariationElement(const RangeVector&);
    const RangeVector& getVariationElement();

    void setStopingCondition(const StopingCondition&);
    const StopingCondition& getStopingCondition();

    void setSolverOptions(const IgoFESolver::SolverOptions&);
    const IgoFESolver::SolverOptions& getSolverOptions();

private:
    FEM * const model;
    StopingCondition stopingCondition;
    IgoFESolver::SolverOptions solverOptions;
    ScaleFactor scale;
    RangeVector variationElement;
    BoolVector changeOfElement;

    CMatrix calculateSens(const EigenModes&,const CMatrix&,const CVector&);
    CVector setLimits(const FEM * const, RangeVector&, const CVector&);
};

#endif // ModelRefining_H
