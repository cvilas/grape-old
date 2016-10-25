//==============================================================================
// File     :   PlotExample.cpp
// Brief    :   Example program for Plot class.
//==============================================================================

#include "PlotExample.h"
#include <QApplication>
#include <QVBoxLayout>

//==============================================================================
PlotExample::PlotExample(QWidget *pParent)
//==============================================================================
    : QWidget(pParent)
{
    setLayout(new QVBoxLayout());
    layout()->addWidget(&_view);
    _view.setNumVisibleSamples(1000);
    _view.setTitle("PlotExample");
    _view.setXLabel("time (s)");
    _view.setYLabel("value");
    _view.setYRangeAuto();
    _view.setXTickWidth(1);
    _view.setYTickWidth(5);
    QStringList legend;
    legend << "sin" << "cos";
    _view.setLegend(legend);

    QObject::connect(&_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    _timer.start(TIMEOUT_MS);
}

//==============================================================================
PlotExample::~PlotExample()
//==============================================================================
{}

//==============================================================================
void PlotExample::onTimer()
//==============================================================================
{
    static double ms = 0;
    ms += TIMEOUT_MS;
    std::array<double, 2> data;
    data[0] = 10 * sin(2 * M_PI * ms/1000.);
    data[1] = 20 * cos(2 * M_PI * ms/1000.);
    _view.addData(ms/1000., data);
}


//==============================================================================
int main(int argc, char *argv[])
//==============================================================================
{
    QApplication a(argc, argv);
    PlotExample w;
    w.resize(640,480);
    w.show();

    return a.exec();
}
