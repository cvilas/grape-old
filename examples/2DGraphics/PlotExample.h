#pragma once

#include "graphics/Plot.h"
#include <QTimer>

class PlotExample : public QWidget
{
    Q_OBJECT
public:
    static constexpr double TIMEOUT_MS = 10;
    PlotExample(QWidget *pParent = nullptr);
    ~PlotExample();
private slots:
    void onTimer();
private:
    grape::Plot<2> _view;
    QTimer _timer;
};
