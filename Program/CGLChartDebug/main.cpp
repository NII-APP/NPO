#include <QApplication>

#include "../CGL/c2dchart.h"
#include "../NPO/engine/afr.h"
#include "../NPO/engine/afrarray.h"
#include "../CGL/cdimensionarray.h"
#include "../CGL/cchartdatalist.h"
#include "../CGL/cslider.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    C2dChart chart;


    AFRArray afr;
    afr.read("C:\\Users\\username\\Downloads\\FRF_10.unv");

    AFRArray* __afr(&afr);
    CChartDataList data;
    const AFR single(__afr->average());
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

    CSlider* const slider(new CSlider);
    slider->setLabelTemplate("%1 hz");
    chart.addSlider(slider);
    CSlider* const slider2(new CSlider);
    slider2->setDragable(0);
    slider2->setPosition(42.8);
    slider2->setColor(Qt::gray);
    chart.addSlider(slider2);

    chart.show();

    return app.exec();
}
