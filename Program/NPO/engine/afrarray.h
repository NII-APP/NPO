#ifndef AFRARRAY_H
#define AFRARRAY_H

#include <vector>

class QString;
class AFR;

class AFRArray : public std::vector<AFR>
{
public:
    AFRArray();
    ~AFRArray();

    void read(const QString& filename, int nodesCount = 0);
    AFR average() const;

    void toChartData(const AFR&);
};

#endif // AFRARRAY_H
