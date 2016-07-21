#include "TestStopWatch.h"

//=============================================================================
TestStopWatch::TestStopWatch()
//=============================================================================
{
}

//-----------------------------------------------------------------------------
void TestStopWatch::resolution()
//-----------------------------------------------------------------------------
{
    grape::StopWatch watch;
    long long int resolution = watch.getResolution();

    qDebug() << " Watch resolution is " << resolution << "nanoseconds";
    QVERIFY(resolution > 0);
}

//-----------------------------------------------------------------------------
void TestStopWatch::period()
//-----------------------------------------------------------------------------
{
    grape::StopWatch watch;
    unsigned long long sleepNs = 1234567890ULL;
    unsigned long long errNsLimit = 1000000; // 1 millisecond

    watch.start();
    grape::StopWatch::nanoSleep(sleepNs);
    watch.stop();

    unsigned long long ns = watch.getTime();
    unsigned long long errNs = ((ns > sleepNs)?(ns - sleepNs):(sleepNs - ns));

    qDebug() << "Elapsed Time: " << ns << " ns. (error: " << errNs << " ns)";
    QVERIFY(errNs < errNsLimit);
}
