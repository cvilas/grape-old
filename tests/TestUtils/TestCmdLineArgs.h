#ifndef TESTCMDLINEARGS_H
#define TESTCMDLINEARGS_H

#include <QString>
#include <QtTest>
#include <utils/CmdLineArgs.h>

//=============================================================================
/// \brief Test class for CmdLineArgs
//=============================================================================
class TestCmdLineArgs : public QObject
{
    Q_OBJECT

public:
    TestCmdLineArgs();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void parse();
};

#endif // TESTCMDLINEARGS_H
