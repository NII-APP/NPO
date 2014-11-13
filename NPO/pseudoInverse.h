#ifndef PSEUDOINVERSE_H
#define PSEUDOINVERSE_H

#include <stdio.h>
#include <conio.h>
#include <iostream>
using namespace std;
#include "geometryform.h"

class MethodInvMat
{
public:
    MethodInvMat();
    ~MethodInvMat();
    int MethodInvMat::pseudoInversion();
    double MethodInvMat::round10 (double value);
    int MethodInvMat::getMatrix(GeometryForm *form);
    double* MethodInvMat::calculateE();
    double MethodInvMat::maxElement(double* A, int n);
    double MethodInvMat::minElement(double* A, int n);
    int MethodInvMat::discreteArray(double* array, int sizeArray, int discrete);
    int MethodInvMat::minElementsNumber(double* A, int n);

<<<<<<< HEAD
private:

    int MethodInvMat::clear0();
    int MethodInvMat::Det(double **matrixIn, int N);
    int MethodInvMat::Print(double ** matrixIn, int rowIn, int columnIn);
    int MethodInvMat::Inversion(double **matrixIn);
    int MethodInvMat::DotTranspose(double **);
    int MethodInvMat::Dot(double ** A, double ** B);

=======
public:
    int MethodInvMat::clear0();
    int Det(double **matrixIn, int N);
    int Print(double ** matrixIn, int rowIn, int columnIn);
    int Inversion(double **matrixIn);
    int DotTranspose(double **);
    int Dot(double ** A, double ** B);
    CGL::CArray calculateE();
    double max(double* A, int n);
    double min(double* A, int n);
>>>>>>> f8622c566568ba145801cea7d79671ae12158133
    double ** matrix;
    double det;
    int row, column;
    double* frequency;
    double* frequencyExp;
    double* deltaE;
};



#endif // PSEUDOINVERSE_H
