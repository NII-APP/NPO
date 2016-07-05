#include "modelupdate.h"

#include <QFile>
#include <QTextStream>

#include <algorithm>
#include <cmath>
#include <iostream>

#include "cvector.h"
#include "cmatrix.h"
#include "material.h"
#include "section/solid.h"


ModelUpdate::ModelUpdate(int numberFreqCountIn,
                         int stepCountIn,
                         double accuracyIn,
                         int numberMatIn,
                         double variationIn,
                         double scaleIn):
        numberFreqCount(numberFreqCountIn),
        stepCount(stepCountIn),
        accuracy(accuracyIn),
        numberMat(numberMatIn),
        variation(variationIn),
        scale(scaleIn)
{

}

void ModelUpdate::setSolverOptions(const IgoFESolver::SolverOptions& o)
{
    options = o;
}

ModelUpdate::~ModelUpdate()
{}

void ModelUpdate::updateFreqByElasticity(FEMPair * data){

    QString filename = "Data.txt";
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream stream(&file);
    file.close();
    FEM *theory = data->theory();
    FEM *practic = data->practic();

    EigenModes modes(theory->getModes());
    if (modes.empty()) {
        modes = IgoFESolver(options).estimateEigenModes(theory);
    }

    CVector deltaFreq(numberFreqCount);
    int numberElements(static_cast<int>(theory->getElements().size()));
    int i(0);
    CMatrix sensitivity(static_cast<int>(numberElements),numberFreqCount);
    CMatrix mFreq(stepCount,numberFreqCount);
    CVector elasticModulus(static_cast<int>(numberElements));
    Material material(Material::MAT1);
    FEM::Materials materials(numberElements);
    std::vector<CRange<double>> rengElasticModulus(numberElements);
    CVector relations;

    for (int k(0); k < numberElements; k++) {
        FinitElement *element = theory->getElements().at(k);
        if (element != nullptr) {
            if (element->getSectionId() < theory->getSections().size()) {
                Section *section = theory->getSection(element->getSectionId());
                materials[k] = theory->getMaterial(section->getMatId());
            }
        }
    }

    i = 0;
    for (int k(0); k < numberElements; k++) {
        FinitElement *element = theory->getElements().at(k);
        if (element != nullptr) {
            int matId = theory->addMaterial(materials[k]);
            Solid *solid = new Solid;
            solid->setProperty(Solid::MID,matId);
            int solidId = theory->addSection(solid);
            element->setSectionId(solidId);
        }
    }

    i = 0;
    for (CVector::iterator it(elasticModulus.begin()); it != elasticModulus.end(); ++it){
        if (materials[i].getType() != Material::Undefined) {
            *it = materials[i].youngModulus();
        } else {
            *it = 0;
        }
        i++;
    }

    for ( int j(0); j < numberElements; j++) {
        (rengElasticModulus.at(j)).setMax( elasticModulus.at(j) * ( 1 + variation/100) );
        (rengElasticModulus.at(j)).setMin( elasticModulus.at(j) * ( 1 - variation/100) );
    }

    for (int step(0); step <= stepCount; step++) {

        file.open(QFile::Append);

        i = 0;
        double averge = 0;
        stream << "Step = " << step;
        for (CVector::iterator it(deltaFreq.begin()); it != deltaFreq.end(); ++it){
            stream << "\r\n";
            mFreq[i][step] = modes[i].frequency();
            stream << step << ")" << '\t' << practic->getModes()[data->relations()[i]].frequency() - modes[i].frequency();
            averge += abs(testFreq[i] - modes[i].frequency()) / numberFreqCount;
            *it = pow(practic->getModes()[data->relations()[i]].frequency(),2) - pow(modes[i].frequency(),2);
            i++;
        }
        if (averge < accuracy){
            return;
        }

        for (int j(0); j < sensitivity.height(); j++) {
            for (int k(0); k < sensitivity.width(); k++) {
                if (materials[k].getType() != Material::Undefined) {
                    sensitivity[j][k] = modes[j].strainEnergy()[k] / materials[k].youngModulus();
                }
                else {
                    sensitivity[j][k] = 0;
                }
            }
        }

        if (step == 1){
            double temp = 0;
            for ( int j(0); j < mFreq.height(); j++ ){
                temp += abs(mFreq[j][step] - mFreq[j][step - 1]) / mFreq[j][step] / mFreq.height();
            }
            scale = 0.1 / temp;
        }

        elasticModulus = elasticModulus + (deltaFreq * (sensitivity.pseudoInversed())) * scale;

        for (int k(0); k < numberElements; k++) {
            FinitElement *element = theory->getElements().at(k);
            if (element != nullptr) {
                if ( !(rengElasticModulus.at(k).belong(elasticModulus.at(k))) ) {
                    double max = rengElasticModulus.at(k).getMax();
                    double min = rengElasticModulus.at(k).getMin();
                    elasticModulus.at(k) = elasticModulus.at(k) >= max ? max : min;
                }
            }
        }

        for (int k(0); k < numberElements; k++) {
            FinitElement *element = theory->getElements().at(k);
            if (element != nullptr) {
                Section *section = theory->getSection(element->getSectionId());
                material[Material::MAT1_NU] =  materials.at(k).poissonRatio();
                material[Material::MAT1_RHO] = materials.at(k).density();
                material[Material::MAT1_E] = elasticModulus[k];
                theory->setMaterial(section->getMatId(),material);
            }
        }

        file.close();

        modes = IgoFESolver(options).estimateEigenModes(theory);
    }

}

void ModelUpdate::updateFreqByElasticity(const FEM * model, const CVector& freq){

    QString filename = "Data.txt";
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    QTextStream stream(&file);
    file.close();

    Q_ASSERT(freq.size() < numberFreqCount);
    CVector testFreq(freq);
    FEM *theory = const_cast<FEM*>(model);

    EigenModes modes(theory->getModes());
    if (modes.empty()) {
        modes = IgoFESolver(options).estimateEigenModes(theory);
    }

    CVector deltaFreq(numberFreqCount);
    int numberElements(static_cast<int>(theory->getElements().size()));
    int i(0);
    CMatrix sensitivity(static_cast<int>(numberElements),numberFreqCount);
    CMatrix mFreq(stepCount,numberFreqCount);
    CVector elasticModulus(static_cast<int>(numberElements));
    Material material(Material::MAT1);
    FEM::Materials materials(numberElements);
    std::vector<CRange<double>> rengElasticModulus(numberElements);

    for (int k(0); k < numberElements; k++) {
        FinitElement *element = theory->getElements().at(k);
        if (element != nullptr) {
            if (element->getSectionId() < theory->getSections().size()) {
                Section *section = theory->getSection(element->getSectionId());
                materials[k] = theory->getMaterial(section->getMatId());
            }
        }
    }

    i = 0;
    for (int k(0); k < numberElements; k++) {
        FinitElement *element = theory->getElements().at(k);
        if (element != nullptr) {
            int matId = theory->addMaterial(materials[k]);
            Solid *solid = new Solid;
            solid->setProperty(Solid::MID,matId);
            int solidId = theory->addSection(solid);
            element->setSectionId(solidId);
        }
    }

    i = 0;
    for (CVector::iterator it(elasticModulus.begin()); it != elasticModulus.end(); ++it){
        if (materials[i].getType() != Material::Undefined) {
            *it = materials[i].youngModulus();
        } else {
            *it = 0;
        }
        i++;
    }

    for ( int j(0); j < numberElements; j++) {
        (rengElasticModulus.at(j)).setMax( elasticModulus.at(j) * ( 1 + variation/100) );
        (rengElasticModulus.at(j)).setMin( elasticModulus.at(j) * ( 1 - variation/100) );
    }

    for (int step(0); step <= stepCount; step++) {

        file.open(QFile::Append);

        i = 0;
        double averge = 0;
        stream << "Step = " << step;
        for (CVector::iterator it(deltaFreq.begin()); it != deltaFreq.end(); ++it){
            stream << "\r\n";
            mFreq[i][step] = modes[i].frequency();
            stream << step << ")" << '\t' << testFreq[i] - modes[i].frequency();
            averge = abs(testFreq[i] - modes[i].frequency());
            *it = pow(testFreq[i],2) - pow(modes[i].frequency(),2);
            i++;
        }
        if (averge / numberFreqCount < accuracy){
            return;
        }

        for (int j(0); j < sensitivity.height(); j++) {
            for (int k(0); k < sensitivity.width(); k++) {
                if (materials[k].getType() != Material::Undefined) {
                    sensitivity[j][k] = modes[j].strainEnergy()[k] / materials[k].youngModulus();
                }
                else {
                    sensitivity[j][k] = 0;
                }
            }
        }

        if (step == 1){
            double temp = 0;
            for ( int j(0); j < mFreq.height(); j++ ){
                temp += abs(mFreq[j][step] - mFreq[j][step - 1]) / mFreq[j][step] / mFreq.height();
            }
            scale = 0.1 / temp;
        }

        elasticModulus = elasticModulus + (deltaFreq * (sensitivity.pseudoInversed())) * scale;

        for (int k(0); k < numberElements; k++) {
            FinitElement *element = theory->getElements().at(k);
            if (element != nullptr) {
                if ( !(rengElasticModulus.at(k).belong(elasticModulus.at(k))) ) {
                    double max = rengElasticModulus.at(k).getMax();
                    double min = rengElasticModulus.at(k).getMin();
                    elasticModulus.at(k) = elasticModulus.at(k) >= max ? max : min;
                }
            }
        }

        for (int k(0); k < numberElements; k++) {
            FinitElement *element = theory->getElements().at(k);
            if (element != nullptr) {
                Section *section = theory->getSection(element->getSectionId());
                material[Material::MAT1_NU] =  materials.at(k).poissonRatio();
                material[Material::MAT1_RHO] = materials.at(k).density();
                material[Material::MAT1_E] = elasticModulus[k];
                theory->setMaterial(section->getMatId(),material);
            }
        }

        file.close();

        modes = IgoFESolver(options).estimateEigenModes(theory);
    }

}
