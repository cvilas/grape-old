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

    grape::SerialPort sp;
    try
    {
        sp.setPortName(_portName);
    }
    catch( grape::Exception &ex )
    {
        QFAIL(ex.what());
    }

    QVERIFY2(_portName == sp.getPortName(), "get not same as set");
}

//-----------------------------------------------------------------------------
void TestSerialPort::openClose()
//-----------------------------------------------------------------------------
{
    grape::SerialPort sp;
    sp.setPortName(_portName);
    try
    {
        sp.open();
    }
    catch(grape::Exception &ex)
    {
        QFAIL(ex.what());
    }
    QVERIFY2(sp.isOpen(), "isOpen after open");
    sp.close();
    QVERIFY2(!sp.isOpen(), "isOpen after close");
}

//-----------------------------------------------------------------------------
void TestSerialPort::baudRate()
//-----------------------------------------------------------------------------
{
    grape::SerialPort sp;
    sp.setPortName(_portName);
    sp.open();
    for(int i = 0; i < grape::SerialPort::BAUD_MAX; ++i)
    {
        try
        {
            sp.setBaudRate((grape::SerialPort::BaudRate)i);
        }
        catch(grape::Exception &ex)
        {
            QFAIL(ex.what());
        }
    }
}

//-----------------------------------------------------------------------------
void TestSerialPort::dataFormat()
//-----------------------------------------------------------------------------
{
    grape::SerialPort sp;
    sp.setPortName(_portName);
    sp.open();
    for(int i = 0; i < grape::SerialPort::DATA_FORMAT_MAX; ++i)
    {
        try
        {
            sp.setDataFormat((grape::SerialPort::DataFormat)i);
        }
        catch(grape::Exception &ex)
        {
            QFAIL(ex.what());
        }
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
