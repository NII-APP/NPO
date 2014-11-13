#ifndef MATERIAL_H
#define MATERIAL_H
#include <vector>
#include <QDebug>

class Material
{
public:
    enum Type {
        MAT1 = 0,
        MAT8 = 1
    };
private:
    Type type;
    std::vector<double> values;

    //static const int VECTOR_SIZES[];
public:
    enum MAT1Indexes {
        MAT1_E = 0,
        MAT1_G = 1,
        MAT1_NU = 2,
        MAT1_RHO = 3,
        MAT1_A = 4,
        MAT1_TREF = 5,
        MAT1_ST = 6,
        MAT1_SC = 7,
        MAT1_SS = 8,
        MAT1_MCSID = 9,

        MAT1_IndexesCount = 10
    };
    enum MAT9Indexes {
        MAT9_E1 = 0,
        MAT9_E2 = 1,
        MAT9_NU12 = 2,
        MAT9_G12 = 3,
        MAT9_G1Z = 4,
        MAT9_G2Z = 5,
        MAT9_RHO = 6,
        MAT9_A1 = 7,
        MAT9_A2 = 8,
        MAT9_TREF = 9,
        MAT9_Xt = 10,
        MAT9_Xc = 11,
        MAT9_Yt = 12,
        MAT9_Yc = 13,
        MAT9_S = 14,
        MAT9_GE = 15,
        MAT9_F12 = 16,
        MAT9_STRN = 17,

        MAT9_IndexesCount = 18
    };

    Material(Type t);
    Material(){}
    double& operator[](int where) { if (values.size() <= where) { values.resize(where + 1, 0.0); } return values[where]; }
};

#endif // MATERIAL_H
