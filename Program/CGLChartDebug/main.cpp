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
    afr.read("C:\\Users\\BOPOHOB\\Downloads\\FRF_10.unv");

    CSlider* const slider(new CSlider);
    slider->setLabelTemplate("%1 hz");
    chart.addSlider(slider);
    /*CSlider* const slider2(new CSlider);
    slider2->setDragable(0);
    slider2->setPosition(42.8);
    slider2->setColor(Qt::gray);
    chart.addSlider(slider2);*/

    chart.showMaximized();

    chart.setData(afr.average().toChartData(AFR::Magnitude));

    return app.exec();
}
