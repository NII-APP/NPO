#ifndef CINDEXES_H
#define CINDEXES_H

#include <vector>

#ifndef NOT_QT_AVAILABLE
class QDebug;
#endif

class CIndexes : public std::vector<int>
{
public:
    CIndexes(const int size = 0, const int defaultValue = 0);
    ~CIndexes();

    static CIndexes indexes(const int size);

    void reduceFrom(const int pos);
};

#ifndef NOT_QT_AVAILABLE
QDebug operator<<(QDebug, const CIndexes&);
#endif

#endif // CINDEXES_H
