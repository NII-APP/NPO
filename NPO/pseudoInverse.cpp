#include "pseudoInverse.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <math.h>
#include "CGL/cgl.h"
using namespace std;





MethodInvMat::MethodInvMat(double ** matrixExternal, int rowExternal, int columnExternal)
{
    row = columnExternal;
    column = rowExternal;
    matrix = matrixExternal;
    det = 1;
};
MethodInvMat::~MethodInvMat()
{}
int MethodInvMat::GetMatrix(GeometryForm* model)
{
   // qDebug() << "create mega matrux";
    int sizeElem = model->elements().size();
    int sizeMode = model->modes().size();

    qDebug() <<
               "sizeElem==" << sizeElem;
    qDebug() <<
               "sizeMode==" << sizeMode;

    CGL::Matrix mat(sizeElem, sizeMode);

    Forms& forms(model->modes());


    for (int i = 0; i < sizeMode; ++i) {
        for (int j = 0; j < sizeElem; ++j) {
            mat[i][j] =  forms[i].power()[j];
        }
    }

  // double** matrix;
  //  matrix = mat.asCMactix();
  //  qDebug() <<
  //            mat[5][4] << "==" << matrix[5][4];


    return 0;
}
int MethodInvMat::Print(double ** matrixIn, int rowIn, int columnIn)
{ //вывод матрицы
    //printf("Output matrix \n");

    cout<<"\n";
    for( int i=0; i<rowIn; i++)
        {
        for(int j=0; j<columnIn; j++)
        printf(" %2.15lf",matrixIn[i][j]);
        cout<<"\n";
        }
    cout<<"\n";

        return 0;
}
int MethodInvMat::DotTranspose(double ** matrixIn)
{
    //умножение матрицы matrix на транспониранную матрицу matrix
    //cout<<"\nDotTranspose\n";
    //__________формирование матрицы с результатами______________//

    for(int i=0; i<column; i++)
        for(int j=0;j<column;j++)
            matrixIn[i][j]=0.0;

        for(int k=0; k<column; k++) {
            for( int i=0; i<column; i++) {
                for(int j=0; j<row; j++) {
                    //cout<<"\nmatrix["<<k<<"]["<<j<<"]="<<matrix[k][j]<<" matrix["<<i<<"]["<<j<<"]="<<matrix[i][j];
                    matrixIn[k][i]+=matrix[k][j]*matrix[i][j];
                }
            }
        }
    //__________формирование матрицы с результатами______________//

            return 0;
}
int MethodInvMat::Inversion(double **matrix)
{ //получение обратной матрицы методом Жордана-Гаусса
    int N = column;
    double temp;


 //--------единичная матрица
    double **E = new double *[N];

    for (int i=0; i<N; i++)
        E[i] = new double [N];

    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++)
        {
            E[i][j] = 0.0;
          if (i == j)
                E[i][j] = 1.0;
        }
 //--------единичная матрица



//---------прямой метод--------------//
    for (int k = 0; k < N; k++)
    {
        temp = matrix[k][k];

    //	Print(E,row,row);
    //	Print(tempM,row,row);
        //cout<<"\ntemp="<<temp;

        for (int j=0; j<N; j++)
        {
            matrix[k][j] /= temp;
            E[k][j] /= temp;
        }


        for (int i=k+1; i<N; i++)
        {
            temp = matrix[i][k];

            for (int j=0; j<N; j++)
            {
                matrix[i][j] -= matrix[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }

    }

//	Print(E,N,N);
//	Print(tempM,row,row);
//---------прямой метод--------------//

  for (int k=N-1; k>0; k--)
    {
        for (int i=k-1; i>=0; i--)
        {
            temp = matrix[i][k];

            for (int j = 0; j < N; j++)
            {
                matrix[i][j] -= matrix[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }

    }

    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            matrix[i][j]=E[i][j];

     for (int i = 0; i < N; i++)
        delete [] E[i];
        delete [] E;

     return 0;
}
int MethodInvMat::Dot(double ** A, double ** B)
{
    //векторное умножение
    //результат записывается в матрицу поступающюю B[]

    double *tempM;
    tempM = new double [row];

    for(int k = 0; k < row; k++)
        for( int i = 0; i < column; i++)
            for(int j = 0; j < column; j++)
            {
                    if( i == 0 )
                    {
                        tempM[j] = B[j][k];
                        B[j][k]=0;
                    }

                B[i][k] += A[i][j] * tempM[j];
            }


        delete [] tempM;

        return 0;
}
int MethodInvMat::PseudoInversion()
{ //вычисление псведообратной матрицы

    double **matrixIn;
    matrixIn = new double * [column];

    for (int i=0; i<column; i++)
    matrixIn[i] = new double[column];

    DotTranspose(matrixIn);
    Inversion(matrixIn);
    Dot(matrixIn,matrix);


    for (int i=0; i<column; i++)
        delete [] matrixIn[i];
        delete [] matrixIn;

    return 0;
}
double MethodInvMat::Round10 (double value)
{
    int i = 10;

    if(value < 10) return 1;

   while((int) value != ((int) value % i) )
      i *= 10;

   return i/10;
}
