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
        MethodInvMat(double ** matrixExternal, int row, int column);
        ~MethodInvMat();
        int PseudoInversion();
        double Round10 (double value);

    private:
        int MethodInvMat::clear0();
        int MethodInvMat::GetMatrix(GeometryForm *form);
        int Det(double **matrixIn, int N);
        int Print(double ** matrixIn, int rowIn, int columnIn);
        int Inversion(double **matrixIn);
        int DotTranspose(double **);
        int Dot(double ** A, double ** B);
        double ** matrix;
        double det;
        int row, column;
};



#endif // PSEUDOINVERSE_H
