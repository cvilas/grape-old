#include "TestSerialPort.h"

//=============================================================================
int main(int argc, char *argv[])
//=============================================================================
{
    TestSerialPort port;
    QTest::qExec(&port, argc, argv);
}

