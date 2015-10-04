#include <QApplication>

#include "../CGL/c2dchart.h"
#include "../NPO/engine/afr.h"
#include "../NPO/engine/afrarray.h"
#include "../CGL/cdimensionarray.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    C2dChart chart;

    AFRArray afr;
    afr.read("C:\\Users\\BOPOHOB\\Downloads\\FRF_10.unv");

    AFRArray* __afr(&afr);
    CChartData::ChartDataList data;
    const AFR single(__afr->avarage());
    if (!single.empty()) {
        data.push_back(CChartData());
        CChartData& top(data.last());
        CDimensionArray* x(new CDimensionArray(static_cast<int>(single.size())));
        CDimensionArray* y(new CDimensionArray(static_cast<int>(single.size())));
        double* xIt(x->data());
        double* yIt(y->data());
        for (AFR::const_iterator it(single.begin()); it != single.end(); ++it, ++xIt, ++yIt) {
            *xIt = it->frequency;
            *yIt = abs(it->amplitude);
        }
        top.push_back(x);
        top.push_back(y);
        x->updateRange();
        y->updateRange();
    }
    chart.setData(data);

    chart.show();

    return app.exec();
}
