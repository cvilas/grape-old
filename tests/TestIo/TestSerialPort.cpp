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
    try
    {
        sp.setPortName(_portName);
    }
    catch( Grape::Exception &ex )
    {
        QFAIL(ex.what());
    }

    QVERIFY2(_portName == sp.getPortName(), "get not same as set");
}

//-----------------------------------------------------------------------------
void TestSerialPort::openClose()
//-----------------------------------------------------------------------------
{
    Grape::SerialPort sp;
    sp.setPortName(_portName);
    try
    {
        sp.open();
    }
    catch(Grape::Exception &ex)
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
    Grape::SerialPort sp;
    sp.setPortName(_portName);
    sp.open();
    for(int i = 0; i < Grape::SerialPort::BAUD_MAX; ++i)
    {
        try
        {
            sp.setBaudRate((Grape::SerialPort::BaudRate)i);
        }
        catch(Grape::Exception &ex)
        {
            QFAIL(ex.what());
        }
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
        try
        {
            sp.setDataFormat((Grape::SerialPort::DataFormat)i);
        }
        catch(Grape::Exception &ex)
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
