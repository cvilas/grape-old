#ifndef TESTSTOPWATCH_H
#define TESTSTOPWATCH_H

#include <QString>
#include <QtTest>
#include <timing/StopWatch.h>

//=============================================================================
/// \brief Test class for StopWatch
//=============================================================================
class TestStopWatch : public QObject
{
    Q_OBJECT

public:
    TestStopWatch();

private Q_SLOTS:
    void resolution();
    void period();
};

#endif // TESTSTOPWATCH_H
