#include "TestCmdLineArgs.h"

//=============================================================================
int main(int argc, char *argv[])
//=============================================================================
{
    TestCmdLineArgs cmdline;
    QTest::qExec(&cmdline, argc, argv);
}

