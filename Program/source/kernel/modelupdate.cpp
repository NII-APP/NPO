#include "modelupdate.h"
#include "femprocessor.h"
#include "cvector.h"
#include "cmatrix.h"
#include "material.h"

ModelUpdate::ModelUpdate(FEMPair *d):
    data(d),
    accuracy(0.02),
    stepCount(7),
    numberFreqCount(3)
{

}

ModelUpdate::~ModelUpdate()
{}

void ModelUpdate::updateFreqByElasticity(){

    FEM *theory = data->theory();
    FEM *practic = data->practic();

    CVector deltaFreq(numberFreqCount);
    int i(0);
    for (CVector::iterator it(deltaFreq.begin()); it != deltaFreq.end(); ++it){
        *it = theory->getModes()[i].frequency() - practic->getModes()[data->relations()[i]].frequency();
        i++;
    }

    CMatrix sensitivity(static_cast<int>(theory->getElements().size()),numberFreqCount);
    for (int j(0); j < sensitivity.height(); j++) {
        for (int k(0); k < sensitivity.width(); k++) {
            sensitivity[j][k] = theory->getModes()[j].strainEnergy()[k] / theory->getMaterial(k).youngModulus();
        }
    }

    i = 0;
    CVector elasticModulus(static_cast<int>(theory->getMaterials().size()));
    for (CVector::iterator it(elasticModulus.begin()); it != elasticModulus.end(); ++it){
        *it = theory->getMaterial(i).youngModulus();
    }

    elasticModulus = elasticModulus + sensitivity.pseudoInversed() * deltaFreq;

    Material material(Material::MAT1);
    for (CVector::iterator it(elasticModulus.begin()); it != elasticModulus.end(); ++it){
        material[Material::MAT1_E] = *it;
        theory->getMaterials().push_back(material);
    }

    FEMProcessor::syncCalculateModes(theory);




}
