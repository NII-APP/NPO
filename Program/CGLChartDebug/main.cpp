#include <QApplication>

#include "../CGL/c2dchart.h"
#include "../NPO/engine/afr.h"
#include "../NPO/engine/afrarray.h"
#include "../CGL/cdimensionarray.h"
#include "../CGL/cchartdatalist.h"
#include "../CGL/cslider.h"
#include <QFile>
#include <QEventLoop>
#include <QTimer>
#include <QVBoxLayout>
#include <QDebug>
#include <QTime>

void w8(int ms) {
    QEventLoop loop;
    QTimer t;
    t.singleShot(ms, &loop, SLOT(quit()));
    loop.exec();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    AFRArray afr;
    afr.read(QFile::exists("C:\\Users\\BOPOHOB\\Downloads\\FRF_METEORIT.unv") ?
                 "C:\\Users\\BOPOHOB\\Downloads\\FRF_METEORIT.unv" :
                 "/user/BOPOHOB/Download/FRF_METEORIT.unv");

    QWidget gui;
    gui.setLayout(new QVBoxLayout);
    C2dChart* chart(new C2dChart(&gui));
    gui.layout()->addWidget(chart);
    QLabel* label(new QLabel(&gui));
    gui.layout()->addWidget(label);

    CSlider* const slider(new CSlider);
    slider->setLabelTemplate("%1 hz source");
    chart->addSlider(slider);

    CSlider* const pos(new CSlider);
    pos->setLabelTemplate("%1 hz pos");
    chart->addSlider(pos);

    CSlider* const left(new CSlider);
    left->setLabelTemplate("%1 hz left");
    chart->addSlider(left);

    CSlider* const right(new CSlider);
    right->setLabelTemplate("%1 hz right");
    chart->addSlider(right);

    chart->showMaximized();

    const AFR average(afr.average());

    chart->setData(average.toChartData(AFR::Amplitude));

    gui.show();

    int i(average.size());
    while (i && chart->isVisible()) {
        --i;
        QTime now(QTime::currentTime());
        slider->setPosition(average.at(i).frequency);

        const int delay(now.msecsTo(QTime::currentTime()));
        QString debug;
        const float damping(average.damping(average.at(i).frequency, debug, pos, left, right));
        label->setText("damping: " + QString::number(damping) + "\ndelay: " + QString::number(delay) + "\n" + debug);
        chart->update();

        w8(1000);
    }

    if (chart->isVisible()) {
        return app.exec();
    } else {
        app.quit();
        return 0;
    }
}
