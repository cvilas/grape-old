#include "TestTimer.h"
#include "timing/StopWatch.h"

//=============================================================================
TestTimer::TestTimer()
//=============================================================================
{
}

//-----------------------------------------------------------------------------
void TestTimer::initTestCase()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void TestTimer::cleanupTestCase()
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
void TestTimer::resolution()
//-----------------------------------------------------------------------------
{
    grape::Timer timer;
    long long int resolution = timer.getResolution();

    qDebug() << " Timer resolution is " << resolution << "nanoseconds";
    QVERIFY(resolution > 0);
}

//-----------------------------------------------------------------------------
void TestTimer::period()
//-----------------------------------------------------------------------------
{
    const unsigned long long NANO = 1000000000ULL;

    grape::Timer timer;
    grape::StopWatch watch;

    // period
    unsigned long long periodNs = 20000000ULL; // 20ms
    unsigned long long maxTicks = 10 * NANO/periodNs; // 10 seconds test
    unsigned long long nTicks = 0;
    unsigned long long timeoutNs = periodNs + NANO;

    // want continuous ticks
    timer.start(periodNs, false);
    watch.start();

    // cycles
    qDebug() << "Ticking " << periodNs << " nanosecs " << maxTicks << " times ";
    while( nTicks < maxTicks )
    {
        // wait for tick
        bool rc = timer.timedWait(timeoutNs);
        QVERIFY2(rc, "timedWait returned false");
        nTicks++;
    }
    watch.stop();
    timer.stop();

    // stats
    unsigned long long avgNs = watch.getTime()/nTicks;
    unsigned long long err = ((avgNs > periodNs)?(avgNs - periodNs):(periodNs - avgNs));

    qDebug() << "Average (" << nTicks << " ticks): " << avgNs << " ns. (error: " << err << " ns)";
    QVERIFY(err < periodNs );
}
