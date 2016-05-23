#ifndef MODELUPDATE_H
#define MODELUPDATE_H


#include "fem.h"
#include "fempair.h"

class ModelUpdate
{

public:
    ModelUpdate(FEMPair *);
    ~ModelUpdate();

    void updateFreqByElasticity();

private:
    FEMPair * const data;
    double accuracy; //точность уточнения
    int stepCount; //количество итераций уточнения
    int numberFreqCount; //количество уточняемых частот

};

#endif // MODELUPDATE_H
