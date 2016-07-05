#ifndef MODELUPDATE_H
#define MODELUPDATE_H

#include "fem.h"
#include "fempair.h"
#include "igofesolver.h"

class ModelUpdate
{

public:
    ModelUpdate(int numberFreqCountIn = 3,
                int stepCountIn = 7,
                double accuracyIn = 0.1,
                int numberMatIn = 20,
                double variationIn = 20,
                double scaleIn = 1);
    ~ModelUpdate();

    void updateFreqByElasticity(FEMPair * pair);
    void updateFreqByElasticity(const FEM *, const CVector& freq);
    void setSolverOptions(const IgoFESolver::SolverOptions&);

private:
    IgoFESolver::SolverOptions options;
    double accuracy; //точность уточнения
    int stepCount; //количество итераций уточнения
    int numberFreqCount; //количество уточняемых частот
    double scale; //масштаб шага уточнения
    int numberMat; //количество новых материалов
    double variation; //масштаб изменения модуля упругости в %

};

#endif // MODELUPDATE_H
