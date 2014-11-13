#include "pseudoInverse.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <math.h>
#include "CGL/cgl.h"
using namespace std;





MethodInvMat::MethodInvMat()
    : det(1.0)
{
}
MethodInvMat::~MethodInvMat()
{}
int MethodInvMat::GetMatrix(GeometryForm* model)
{
    CGL::CArray elasticy(model->extractElasticityModulus());
   // qDebug() << "create mega matrux";
    int sizeElem = model->elements().size();
    int sizeMode = model->modes().size();

    qDebug() << "sizeElem ==" << sizeElem;
    qDebug() << "sizeMode ==" << sizeMode;

    row = sizeElem;
    column = sizeMode;// sizeMode;

    matrix = new double* [column];
    for (int i(0); i != column; ++i) {
        matrix[i] = new double[row];
    }

    frequency = new double[sizeMode];
    frequencyExp = new double[sizeMode];

    Forms& forms(model->modes());

    //qDebug() << "memory alocated" << sizeElem << "rows and" << sizeMode << "columns";
    //qDebug() << "forms size =" << forms.size() << "forms first size" << forms.front().power().size();

    for (int i = 0; i < sizeElem; ++i) {
        for (int j = 0; j < sizeMode; ++j) {
            matrix[j][i] =  forms[j].power()[i] / (elasticy[i] ? elasticy[i] : 1.0);
        }
    }

    for (int i(0); i != sizeMode; ++i) {
        frequencyExp[i] = forms[i].frequency();
    }

    for (int i(0); i != sizeMode; ++i) {
        frequency[i] = forms[i].frequency() + 1.5*i;
    }

    return 0;
}
int MethodInvMat::Print(double ** matrixIn, int rowIn, int columnIn)
{ //вывод матрицы
    //printf("Output matrix \n");

    cout<<"\n";
    for( int i=0; i<rowIn; i++) {
        for(int j=0; j<columnIn; j++)
            cout << matrixIn[i][j]<<"  ";
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

    for (int i = 0; i < column; i++) {
        for (int j=0; j < column; j++)
            matrixIn[i][j]=0.0;
    }


    for (int k=0; k<column; k++) {
        for ( int i=0; i<column; i++) {
            for (int j=0; j<row; j++) {
                //cout<<"\nmatrix["<<k<<"]["<<j<<"]="<<matrix[k][j]<<" matrix["<<i<<"]["<<j<<"]="<<matrix[i][j];
                matrixIn[k][i] += matrix[k][j] * matrix[i][j];
            }
        }
    }

     //__________формирование матрицы с результатами______________//

            return 0;
}
int MethodInvMat::Inversion(double** matrix)
{ //получение обратной матрицы методом Жордана-Гаусса
    int N = column;
    double temp;


 //--------единичная матрица
    double** E = new double *[N];

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

    for (int k=N-1; k>0; k--) {
        for (int i=k-1; i>=0; i--) {
            temp = matrix[i][k];

            for (int j = 0; j < N; j++) {
                matrix[i][j] -= matrix[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }

    for(int i=0;i<N;i++) {
        for(int j=0;j<N;j++) {
            matrix[i][j]=E[i][j];
        }
    }

     for (int i = 0; i < N; i++) {
        delete [] E[i];
     }
     delete [] E;

     printf("\nDAN INVERSE");

     return 0;


}

int MethodInvMat::Dot(double ** A, double ** B)
{
    //векторное умножение
    //результат записывается в матрицу поступающюю B[]

    double *tempM;
    tempM = new double [row];

     qDebug() << "\nrow = "<<row;
     qDebug() << "\ncolumn = "<<column;

    for(int k = 0; k < row; k++)
        for( int i = 0; i < column; i++)
            for(int j = 0; j < column; j++)
            {
                if( i == 0 )
                {
                    tempM[j] = B[j][k];
                    B[j][k] = 0;
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
    for (int i=0; i<column; i++) {
        matrixIn[i] = new double[column];
    }



    DotTranspose(matrixIn);
    Inversion(matrixIn);
    Dot(matrixIn,matrix);
    calculateE();


    for (int i=0; i<column; i++) {
        delete [] matrixIn[i];
    }
    delete [] matrixIn;

    return 0;
}

CGL::CArray MethodInvMat::calculateE()
{
    DotTranspose(matrixIn);
    Inversion(matrixIn);
    Dot(matrixIn,matrix);

    deltaE = new double[row];
    double tempFrec;

    double kof = 100;

    for (int i = 0; i < row; ++i) {
       deltaE[i] = 0;
        for (int j = 0; j < column; ++j) {
            tempFrec = frequencyExp[j] - frequency[j];
            deltaE[i] += kof * matrix[j][i] * tempFrec ;
        }
    }

    CGL::CArray result(deltaE, row);

    qDebug() << result.estimateRange();

    return result;
}

double MethodInvMat::Round10 (double value)
{
    int i = 10;

    if(value < 10) return 1;

   while((int) value != ((int) value % i) )
      i *= 10;

   return i/10;
}

double MethodInvMat::max(double* A, int n)
{
int i;
double max;

max = 0.0;

for (i = 0; i < n; i++)
if ( max < A[i] ) max = A[i];


return( max );
}

double MethodInvMat::min(double* A, int n)
{
int i;
double min;

min = 1.e30;

for (i = 0; i < n; i++)
if ( min > A[i] ) min = A[i];


return( min );
}
