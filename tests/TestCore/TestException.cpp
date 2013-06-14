#include "TestException.h"
#include "core/Exception.h"
#include <errno.h>

//=============================================================================
TestException::TestException()
//=============================================================================
{
}

//-----------------------------------------------------------------------------
void TestException::initTestCase()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void TestException::cleanupTestCase()
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
void TestException::enterNumberBelowFive(int number)
//-----------------------------------------------------------------------------
{
    // throw an exception for invalid argument
    if(number >= 5)
    {
        throw Grape::Exception(EINVAL,"[enterNumberBelowFive]");
    }
}

//-----------------------------------------------------------------------------
void TestException::tryNoFault()
//-----------------------------------------------------------------------------
{
    try
    {
        enterNumberBelowFive(2);
    }

    catch( Grape::Exception& ex ) // prefer to catch by reference where possible
    {
        qDebug() << "caught exception: " << ex.what() << ": " << strerror(ex.code());
        QFAIL("No Exception case");
    }
}

//-----------------------------------------------------------------------------
void TestException::tryFault()
//-----------------------------------------------------------------------------
{
    try
    {
        enterNumberBelowFive(10);   // this will throw exception
        QFAIL("Exception not thrown");
    }

    catch( Grape::Exception& ex ) // prefer to catch by reference where possible
    {
        qDebug() << "caught exception: " << ex.what() << ": " << strerror(ex.code());
        return;
    }
    QFAIL("Exception not caught");
}

