#ifndef MODELUPDATE_H
#define MODELUPDATE_H


#include "fem.h"
#include "fempair.h"

class ModelUpdate
{

public:
    ModelUpdate(FEMPair *);
    ModelUpdate(const FEM *, const CVector& freq);
    ~ModelUpdate();

    void updateFreqByElasticity();

private:
    FEMPair * const data;
    const FEM * model;
    CVector testFreq; // вектор экспериментальных частот
    double accuracy; //точность уточнения
    int stepCount; //количество итераций уточнения
    int numberFreqCount; //количество уточняемых частот
    double scale; //масштаб шага уточнения
    int numberMat; //количество новых материалов
    double variation; //масштаб изменения модуля упругости в %

};

#endif // MODELUPDATE_H
