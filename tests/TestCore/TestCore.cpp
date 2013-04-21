#include "TestException.h"

//=============================================================================
int main(int argc, char *argv[])
//=============================================================================
{
    TestException ex;
    QTest::qExec(&ex, argc, argv);
}

