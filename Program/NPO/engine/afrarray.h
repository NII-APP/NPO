#ifndef AFRARRAY_H
#define AFRARRAY_H

#include <vector>

class QString;
class AFR;
class CChartDataList;

class AFRArray : public std::vector<AFR>
{
public:
    AFRArray();
    ~AFRArray();

    void read(const QString& filename, int nodesCount = 0);
    AFR average() const;

    CChartDataList toChartData(unsigned interalParts = 0x7) const;
};

#endif // AFRARRAY_H
