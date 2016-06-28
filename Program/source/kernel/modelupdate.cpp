#include <QFile>
#include <QTextStream>

#include "modelupdate.h"
#include "femprocessor.h"
#include "cvector.h"
#include "cmatrix.h"
#include "material.h"
#include "section/solid.h"
#include <algorithm>
#include <math.h>

#include <iostream>

ModelUpdate::ModelUpdate(FEMPair *d):
    data(d),
    model(nullptr),
    accuracy(0.02),
    stepCount(10),
    numberFreqCount(6),
    scale(1.),
    numberMat(20),
    variation(90)
{

}

ModelUpdate::ModelUpdate(const FEM *m, const CVector& freq):
    data(nullptr),
    model(m),
    accuracy(0.02),
    stepCount(10),
    testFreq(freq),
    numberFreqCount(freq.size()),
    scale(1.),
    numberMat(20),
    variation(90.)
{

}

ModelUpdate::~ModelUpdate()
{}

void ModelUpdate::updateFreqByElasticity(){

    QString filename = "Data.txt";
    QFile file( filename );
    QTextStream stream( &file );
    file.open(QFile::WriteOnly);
    file.close();
    FEM *theory = nullptr;
    FEM *practic = nullptr;

    if ( data != nullptr) {
        theory = data->theory();
        practic = data->practic();
    } else {
        theory = const_cast<FEM *>(model);
    }

    if (theory->getModes().empty()) {
        FEMProcessor::syncCalculateModes(theory);
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

    //Считывание материала элементов в массив materials
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
        qDebug() << "STEP = "<< step;

        //в дальнейшем нужно будет пересчитывать relations(), т.к. как СФ могут значительно изменяться
        //в ходе уточнения модели
        i = 0;
        for (CVector::iterator it(deltaFreq.begin()); it != deltaFreq.end(); ++it){
            stream << "\r\n";
            mFreq[i][step] = theory->getModes()[i].frequency();
            if ( data != nullptr) {
                stream << step << ")" << '\t' << practic->getModes()[data->relations()[i]].frequency() - theory->getModes()[i].frequency();
                *it = pow(practic->getModes()[data->relations()[i]].frequency(),2) - pow(theory->getModes()[i].frequency(),2);
            } else {
                stream << step << ")" << '\t' << testFreq[i] - theory->getModes()[i].frequency();
                *it = pow(testFreq[i],2) - pow(theory->getModes()[i].frequency(),2);
            }
            i++;
        }

        qDebug() << "sensitivity";
        for (int j(0); j < sensitivity.height(); j++) {
            for (int k(0); k < sensitivity.width(); k++) {
                if (materials[k].getType() != Material::Undefined) {
                    sensitivity[j][k] = theory->getModes()[j].strainEnergy()[k] / materials[k].youngModulus();
                }
                else {
                    sensitivity[j][k] = 0;
                }
            }
        }

        qDebug() << "scale";
        if (step == 1){
            double temp = 0;
            for ( int j(0); j < mFreq.height(); j++ ){
                temp += abs(mFreq[j][step] - mFreq[j][step - 1]) / mFreq[j][step] / mFreq.height();
            }
            scale = 0.1 / temp;
        }

        qDebug() << "pseudoInversed";
        elasticModulus = elasticModulus + (deltaFreq * (sensitivity.pseudoInversed())) * scale;

        qDebug() << "elasticModulus.at(k) = elasticModulus.at(k) >= max ? max : min";
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

        qDebug() << "setMaterial";
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

        qDebug() << "syncCalculateModes";
        file.close();
        FEMProcessor::iteration = step + 1;

        theory->getModes().clear();
        FEMProcessor::syncCalculateModes(theory);
    }

}
