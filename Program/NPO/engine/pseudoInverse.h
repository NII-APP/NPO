#ifndef PSEUDOINVERSE_H
#define PSEUDOINVERSE_H

#include <stdio.h>
#include <conio.h>
#include <iostream>
using namespace std;
#include "eigenmodes.h"

class MethodInvMat
{
public:
    int discreteArray(double* arrayIn, int sizeArray, int discrete);
    int minElementsNumber(double* A, int n);
    MethodInvMat();
    ~MethodInvMat();
    int PseudoInversion();
    double Round10 (double value);
    int GetMatrix(MeshForm *form);

public:
    int clear0();
    int Det(double **matrixIn, int N);
    int Print(double ** matrixIn, int rowIn, int columnIn);
    int Inversion(double **matrixIn);
    int DotTranspose(double **);
    int Dot(double ** A, double ** B);
    CGL::CArray calculateE();
    double max(double* A, int n);
    double min(double* A, int n);
    double ** matrix;
    double det;
    int row, column;
    double* frequency;
    double* frequencyExp;
    double* deltaE;
};



#endif // PSEUDOINVERSE_H
