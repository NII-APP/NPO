#ifndef CCHART_H
#define CCHART_H
#include "cglwidget.h"

class CChartData;
class CChart : public CGLWidget
{
    const CChartData* d;
public:
    explicit CChart(QWidget* parent = 0);

    /**
     * @brief setData set the data to chart. Set this as a parent QObject
     * @param data
     */
    void setData(CChartData* data);

    /**
     * @brief setData set the data to chart. And nothing more
     * @param const data
     */
    void setData(const CChartData* data);

    /**
     * @brief data
     * @return current data as a const pointer
     */
    const CChartData* data();
};

#endif // CCHART_H
