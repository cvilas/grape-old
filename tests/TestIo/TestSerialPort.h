#include <QString>
#include <QtTest>
#include <io/SerialPort.h>

//=============================================================================
/// \brief Test class for SerialPort
//=============================================================================
class TestSerialPort : public QObject
{
    Q_OBJECT

public:
    TestSerialPort();

private Q_SLOTS:
    void cleanupTestCase();
    void initTestCase();
    void portName();
    void openClose();
    void baudRate();
    void dataFormat();
    //void readWrite();
private:
    std::string _portName;
};

