#include "modelrefining.h"

#include <QFile>
#include <QTextStream>

#include <algorithm>
#include <cmath>
#include <iostream>

#include "cvector.h"
#include "cmatrix.h"
#include "material.h"
#include "section/solid.h"

ModelRefining::ModelRefining(const FEM * modelIn)
    : model(const_cast<FEM*>(modelIn))
    , variationElement(modelIn->getElements().size())
    , changeOfElement(modelIn->getElements().size())
    , scale([](const CVector& p, const CVector& pPrev, const CVector& absoluteOptimumIn)->double{
                CVector absoluteOptimum(absoluteOptimumIn);
                if (pPrev.empty()){
                    return 1;
                } else {
                    return abs(absoluteOptimum.average() / (p - pPrev).average());
                }
            })
{
    for (RangeVector::iterator it(variationElement.begin()); it != variationElement.end(); ++it) {
        *it = CRealRange(-0.9,0.9);
    }

    for (int k(0); k < modelIn->getElements().size(); k++) {
        if (modelIn->getElements().at(k) != nullptr) {
            changeOfElement[k] = true;
        } else {
            changeOfElement[k] = false;
        }
    }
}

ModelRefining::~ModelRefining()
{}

ModelRefining::StopingCondition::StopingCondition()
    : isStepCountCondition(true)
    , isAccuracyCondition(false)
    , isStognationCondition(false)

    , stepCount(7)
    , accuracy(0.1)
    , stognation(10)
    , stognationLength(3)
{}

bool ModelRefining::StopingCondition::stop(const CVector& vecAccuracyIn){
    int currentStepCount(static_cast<int>(vecAccuracyIn.size()));
    bool isStep(false);
    bool isAccuracy(false);
    bool isStognation(false);

    if (isStepCountCondition) {
        isStep = currentStepCount >= stepCount;
    }
    if(isAccuracyCondition && !vecAccuracyIn.empty()) {
        isAccuracy = vecAccuracyIn.back() < accuracy;
    }
    if(isStognationCondition && vecAccuracyIn.size() >=  stognationLength) {
        CVector vDisper(stognationLength);
        std::copy(vecAccuracyIn.end() - stognationLength, vecAccuracyIn.end(), vDisper.begin());
        vDisper = vDisper - vDisper.average();
        isStognation = vDisper.abs().estimateRange().getMax() < stognation;
    }

    return !(isStep || isAccuracy || isStognation);
}

void ModelRefining::updateFreqByElasticity(const CVector& testFreq)
{
    EigenModes modes(model->getModes());
    if (modes.empty()) {
        modes = IgoFESolver(solverOptions).estimateEigenModes(model);
    }

    int numberFreq(static_cast<int>(testFreq.size()));
    int numberElements(static_cast<int>(model->getElements().size()));
    double scaleValue(1);
    CMatrix sensitivity(numberElements,numberFreq);

    CVector oldFreq;
    CVector currentFreq(numberFreq);
    CVector deltaFreq(numberFreq);
    CVector vectAccuracy;

    CVector elasticModulus(numberElements);
    RangeVector rengElasticModulus(numberElements);

    for (int k(0); k < numberElements; k++) {
        FinitElement *element = model->getElements().at(k);
        if (element != nullptr) {
            Section *section = model->getSection(element->getSectionId());
            Material material(model->getMaterial(section->getMatId()));
            if (material.getType() != Material::Undefined) {
                elasticModulus[k] = material.youngModulus();
            } else {
                elasticModulus[k] = 0.0;
            }
        } else {
            elasticModulus[k] = 0.0;
        }
    }

    for (int k(0); k < numberElements; k++) {
       FinitElement *element = model->getElements().at(k);
       if (element != nullptr) {
           Section *section = model->getSection(element->getSectionId());
           Material material(model->getMaterial(section->getMatId()));
           int matId = model->addMaterial(material);
           Solid *solid = new Solid;
           solid->setProperty(Solid::MID,matId);
           int solidId = model->addSection(solid);
           element->setSectionId(solidId);
       }
    }

    for ( int j(0); j < numberElements; j++) {
        rengElasticModulus.at(j).include({ elasticModulus.at(j) * (1 + variationElement.at(j).getMax()),
                                           elasticModulus.at(j) * (1 + variationElement.at(j).getMin())});
    }

    while ( stopingCondition.stop(vectAccuracy) )
    {
        int i(0);
        double averge = 0;
        for (CVector::iterator it(deltaFreq.begin()); it != deltaFreq.end(); ++it){
            currentFreq[i] = modes[i].frequency();
            averge += abs(testFreq[i] - modes[i].frequency());
            *it = pow(testFreq[i],2) - pow(modes[i].frequency(),2);
            i++;
        }

        vectAccuracy.push_back(averge / i);

        sensitivity = calculateSens(modes,sensitivity,elasticModulus);

        if(vectAccuracy.size() == 2){
            scaleValue = scale(currentFreq,oldFreq,testFreq - currentFreq);
        }

        elasticModulus = elasticModulus + (deltaFreq * (sensitivity.pseudoInversed()))
                                            * scaleValue;

        elasticModulus = setLimits(model,rengElasticModulus,elasticModulus);

        for (int k(0); k < numberElements; k++) {
            FinitElement *element = model->getElements().at(k);
            if (element != nullptr && changeOfElement[k]) {
                Section *section = model->getSection(element->getSectionId());
                Material material(model->getMaterial(section->getMatId()));
                material[Material::MAT1_E] = elasticModulus[k];
                model->setMaterial(section->getMatId(),material);
            }
        }

        oldFreq = currentFreq;

        modes = IgoFESolver(solverOptions).estimateEigenModes(model);
    }

}

CVector ModelRefining::setLimits( const FEM * const modelIn
                              , RangeVector &rengElasticModulusIn
                              , const CVector &elasticModulusIn) {

    CVector elasticModulus(elasticModulusIn);
    for (int k(0); k < modelIn->getElements().size(); k++) {
        FinitElement *element = modelIn->getElements().at(k);
        if (element != nullptr) {
            if ( !(rengElasticModulusIn.at(k).belong(elasticModulus.at(k))) ) {
                double max = rengElasticModulusIn.at(k).getMax();
                double min = rengElasticModulusIn.at(k).getMin();
                elasticModulus.at(k) = elasticModulus.at(k) >= max ? max : min;
            }
        }
    }

    return elasticModulus;
}


CMatrix ModelRefining::calculateSens(const EigenModes &modes,const CMatrix &sensitivity,const CVector &materials) {
    CMatrix tempSens(sensitivity);
    for (int j(0); j < tempSens.height(); j++) {
        for (int k(0); k < tempSens.width(); k++) {
            if (materials[k] != 0) {
                tempSens[j][k] = modes[j].strainEnergy()[k] / materials[k];
            }
            else {
                tempSens[j][k] = 0;
            }
        }
    }
    return tempSens;
}

void ModelRefining::setSolverOptions(const IgoFESolver::SolverOptions& o)
{
    solverOptions = o;
}
const IgoFESolver::SolverOptions& ModelRefining::getSolverOptions(){
    return solverOptions;
}

void ModelRefining::setStopingCondition(const StopingCondition& stopingConditionIn){
    stopingCondition = stopingConditionIn;
}
const ModelRefining::StopingCondition& ModelRefining::getStopingCondition(){
    return stopingCondition;
}

void ModelRefining::setVariationElement(const RangeVector& variationElementIn){
    variationElement = variationElementIn;
}
const RangeVector& ModelRefining::getVariationElement(){
    return variationElement;
}

void ModelRefining::setChangeOfElement(const BoolVector& changeOfElementIn){
    changeOfElement = changeOfElementIn;
}
const BoolVector& ModelRefining::getChangeOfElement(){
    return changeOfElement;
}

void ModelRefining::setScaleFactor(const ScaleFactor& scaleFactorIn){
    scale = scaleFactorIn;
}
const ScaleFactor& ModelRefining::getScaleFactor(){
    return scale;
}
