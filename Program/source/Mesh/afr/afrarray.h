#ifndef AFRARRAY_H
#define AFRARRAY_H

#include <vector>
#include "afr.h"

class QString;
class CChartDataList;
class EigenMode;

class AFRArray : public std::vector<AFR>
{
public:
    AFRArray();
    ~AFRArray();

    void read(const QString& filename, int nodesCount = 0);
    AFR average() const;
    EigenMode getMode(const double freq) const;

    CChartDataList toChartData(unsigned interalParts = 0x7) const;
};

#endif // AFRARRAY_H
