#ifndef TESTEXCEPTION_H
#define TESTEXCEPTION_H

#include <QString>
#include <QtTest>
#include <core/Exception.h>

//=============================================================================
/// \brief Test class for Exception
//=============================================================================
class TestException : public QObject
{
    Q_OBJECT

public:
    TestException();

    // Just an example function
    void enterNumberBelowFive(int number);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void tryNoFault();
    void tryFault();
};

#endif // TESTEXCEPTION_H
