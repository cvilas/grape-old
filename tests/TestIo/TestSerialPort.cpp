#include "TestSerialPort.h"

//=============================================================================
TestSerialPort::TestSerialPort()
//=============================================================================
{
}

//-----------------------------------------------------------------------------
void TestSerialPort::initTestCase()
//-----------------------------------------------------------------------------
{
#ifdef WIN32
    _portName = "COM6";
#else
    _portName = "/dev/ttyUSB0";
#endif

}

//-----------------------------------------------------------------------------
void TestSerialPort::cleanupTestCase()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void TestSerialPort::portName()
//-----------------------------------------------------------------------------
{
    // verify set/get portname

    Grape::SerialPort sp;
    QVERIFY2(sp.setPortName(_portName), "Error setting port name");
    QVERIFY2(_portName == sp.getPortName(), "get not same as set");
}

//-----------------------------------------------------------------------------
void TestSerialPort::openClose()
//-----------------------------------------------------------------------------
{
    Grape::SerialPort sp;
    sp.setPortName(_portName);
    QVERIFY2(sp.open(), sp.lastError.getMessage().c_str());
    QVERIFY2(sp.isOpen(), "isOpen after open");
    sp.close();
    QVERIFY2(!sp.isOpen(), "isOpen after close");
}

//-----------------------------------------------------------------------------
void TestSerialPort::baudRate()
//-----------------------------------------------------------------------------
{
    Grape::SerialPort sp;
    sp.setPortName(_portName);
    sp.open();
    for(int i = 0; i < Grape::SerialPort::BAUD_MAX; ++i)
    {
        QVERIFY2(sp.setBaudRate((Grape::SerialPort::BaudRate)i), sp.lastError.getMessage().c_str());
    }
}

//-----------------------------------------------------------------------------
void TestSerialPort::dataFormat()
//-----------------------------------------------------------------------------
{
    Grape::SerialPort sp;
    sp.setPortName(_portName);
    sp.open();
    for(int i = 0; i < Grape::SerialPort::DATA_FORMAT_MAX; ++i)
    {
        QVERIFY2(sp.setDataFormat((Grape::SerialPort::DataFormat)i), sp.lastError.getMessage().c_str());
    }
}
/*
//-----------------------------------------------------------------------------
void TestSerialPort::readWrite()
//-----------------------------------------------------------------------------
{
    // ** This test requires hardware to be hooked up ***

    Grape::SerialPort sp;
    sp.setPortName(_portName);
    sp.open();
    if( !sp.isOpen() )
    {
        QFAIL(sp.lastError.getMessage().c_str());
    }
    if( !sp.setBaudRate(Grape::SerialPort::B115200) )
    {
        QFAIL(sp.lastError.getMessage().c_str());
    }
    if( !sp.setDataFormat(Grape::SerialPort::D8N1) )
    {
        QFAIL(sp.lastError.getMessage().c_str());
    }
}
*/
