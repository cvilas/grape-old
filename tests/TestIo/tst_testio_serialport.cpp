#include <QString>
#include <QtTest>
#include <io/SerialPort.h>

//=============================================================================
/// \class Test class for SerialPort
//=============================================================================
class TestIo_SerialPort : public QObject
{
    Q_OBJECT
    
public:
    TestIo_SerialPort();
    
private Q_SLOTS:
    void cleanupTestCase();
    void initTestCase();
    void portName();
    void openClose();
    void baudRate();
    void dataFormat();
    void readWrite();
private:
    std::string _portName;
};

//=============================================================================
TestIo_SerialPort::TestIo_SerialPort()
//=============================================================================
{
}

//-----------------------------------------------------------------------------
void TestIo_SerialPort::initTestCase()
//-----------------------------------------------------------------------------
{
#ifdef WIN32
    _portName = "COM6";
#else
    _portName = "/dev/ttyUSB0";
#endif

}

//-----------------------------------------------------------------------------
void TestIo_SerialPort::cleanupTestCase()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void TestIo_SerialPort::portName()
//-----------------------------------------------------------------------------
{
    // verify set/get portname

    Grape::SerialPort sp;
    QVERIFY2(sp.setPortName(_portName), "Error setting port name");
    QVERIFY2(_portName == sp.getPortName(), "get not same as set");
}

//-----------------------------------------------------------------------------
void TestIo_SerialPort::openClose()
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
void TestIo_SerialPort::baudRate()
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
void TestIo_SerialPort::dataFormat()
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

//-----------------------------------------------------------------------------
void TestIo_SerialPort::readWrite()
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

QTEST_APPLESS_MAIN(TestIo_SerialPort)

#include "tst_testio_serialport.moc"
