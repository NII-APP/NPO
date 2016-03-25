#ifndef CINDEXES_H
#define CINDEXES_H

#include <vector>
class QDebug;

class CIndexes : public std::vector<int>
{
public:
    CIndexes(const int size = 0, const int defaultValue = 0);
    ~CIndexes();

    static CIndexes indexes(const int size);

    void reduceFrom(const int pos);
};

QDebug operator<<(QDebug, const CIndexes&);

#endif // CINDEXES_H
