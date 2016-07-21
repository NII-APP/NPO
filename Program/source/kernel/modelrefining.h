#ifndef ModelRefining_H
#define ModelRefining_H

#include "fem.h"
#include "fempair.h"
#include "igofesolver.h"
#include <functional>
#include "crange.h"

typedef std::function<double(const CVector&,const CVector&,const CVector&, const double&)> ScaleFactor;
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

    void setScaleFactor(const ScaleFactor&);
    const ScaleFactor& getScaleFactor() const;

    void setHeuristicCoefScale(const double&);
    const double& getHeuristicCoefScale() const;

    void setChangeOfElement(const BoolVector&);
    const BoolVector& getChangeOfElement() const;

    void setVariationElement(const RangeVector&);
    const RangeVector& getVariationElement() const;

    void setStopingCondition(const StopingCondition&);
    const StopingCondition& getStopingCondition() const;

    void setSolverOptions(const IgoFESolver::SolverOptions&);
    const IgoFESolver::SolverOptions& getSolverOptions() const;

private:
    FEM * const model;
    StopingCondition stopingCondition;
    IgoFESolver::SolverOptions solverOptions;
    ScaleFactor scale;
    RangeVector variationElement;
    BoolVector changeOfElement;
    double heuristicCoefScale;

    CMatrix calculateSens(const EigenModes&,const CMatrix&,const CVector&);
    CVector setLimits(const FEM * const, RangeVector&, const CVector&);
};

#endif // ModelRefining_H
