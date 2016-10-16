#include "cindexes.h"

#ifndef NOT_QT_AVAILABLE
#include <QDebug>
#endif

CIndexes::CIndexes(const int size, const int defaultValue)
    : std::vector<int>(size, defaultValue)
{

}

CIndexes::~CIndexes()
{

}

CIndexes CIndexes::indexes(const int size)
{
    CIndexes r(size);
    for (int i(0); i != size; ++i) {
        r[i] = i;
    }
    return r;
}

void CIndexes::reduceFrom(const int pos)
{
    CIndexes::iterator it(begin() + pos);
    while (it < end()) {
        --*it;
        ++it;
    }
}

#ifndef NOT_QT_AVAILABLE
QDebug operator<<(QDebug out, const CIndexes& m)
{
    switch (m.size()) {
    case 0:
        out << "CIndexes( 0 ) { }";
        return out;
    case 1:
        out << "CIndexes( 1 ) {" << m.front() << "}";
        return out;
    case 2:
        out << "CIndexes( 2 ) {" << m[0] << ',' << m[1] << "}";
        return out;
    case 3:
        out << "CIndexes( 3 ) {" << m[0] << ',' << m[1] << ',' << m[2] << "}";
        return out;
    case 4:
        out << "CIndexes( 4 ) {" << m[0] << ',' << m[1] << ',' << m[2] << ',' << m[3] << "}";
        return out;
    case 5:
        out << "CIndexes( 5 ) {" << m[0] << ',' << m[1] << ',' << m[2] << ',' << m[3] << ',' << m[4] << "}";
        return out;
    case 6:
        out << "CIndexes( 6 ) {" << m[0] << ',' << m[1] << ',' << m[2] << ',' << m[3] << ',' << m[4] << ',' << m[5] << "}";
        return out;
    default:
        out << "CIndexes(" << m.size() << ") {" << m[0] << ',' << m[1] << ',' << m[2]
            << "..." << m[m.size() - 3] << ',' << m[m.size() - 2] << ',' << m[m.size() - 1] << "}";
        return out;
    }
}
#endif
