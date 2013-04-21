#include "TestStopWatch.h"
#include "TestTimer.h"

//=============================================================================
int main(int argc, char *argv[])
//=============================================================================
{
    TestStopWatch watch;
    QTest::qExec(&watch, argc, argv);

    TestTimer timer;
    QTest::qExec(&timer, argc, argv);
}

