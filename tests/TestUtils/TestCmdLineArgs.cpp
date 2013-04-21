#include "TestCmdLineArgs.h"
#include "utils/CmdLineArgs.h"

//=============================================================================
TestCmdLineArgs::TestCmdLineArgs()
//=============================================================================
{
}

//-----------------------------------------------------------------------------
void TestCmdLineArgs::initTestCase()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void TestCmdLineArgs::cleanupTestCase()
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
void TestCmdLineArgs::parse()
//-----------------------------------------------------------------------------
{
    char *argv[] = {"program name", "-name", "Paul", "Smith", "-phone", "019081234567"};
    int argc = sizeof(argv) / sizeof(char*);

    Grape::CmdLineArgs parser(argc, argv);
    int n = parser.getNumOptions();

    QVERIFY2(n==2, "Failed getNumOptions");

    std::string opt;
    std::string val;

    parser.getOptionByIndex(0, opt, val);
    QCOMPARE(val.c_str(), "Paul Smith");
    QCOMPARE(opt.c_str(), "name");

    parser.getOptionByIndex(1, opt, val);
    QCOMPARE(val.c_str(), "019081234567");
    QCOMPARE(opt.c_str(), "phone");

    for(int i = 0; i < n; ++i )
    {
        if( parser.getOptionByIndex(i, opt, val) )
        {
            qDebug() << "Option: " << opt.c_str() << " Value: " << val.c_str();
        }
    }
}
