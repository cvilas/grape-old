#ifndef TESTTIMER_H
#define TESTTIMER_H

#include <QString>
#include <QtTest>
#include <timing/Timer.h>

//=============================================================================
/// \brief Test class for Timer
//=============================================================================
class TestTimer : public QObject
{
    Q_OBJECT

public:
    TestTimer();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void resolution();
    void period();
};

#endif // TESTTIMER_H
