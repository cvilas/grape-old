//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SerialPortException.h
// Brief    : Exceptions thrown by serial port
//==============================================================================

#ifndef GRAPE_SERIALPORTEXCEPTION_H
#define GRAPE_SERIALPORTEXCEPTION_H

#include "grapeio_common.h"
#include "IoException.h"

namespace Grape
{

/// \brief common base class for exceptions thrown by serial port classes
/// \ingroup io
class GRAPEIO_DLL_API SerialPortException : public Grape::IoException
{
public:
    SerialPortException() throw() : IoException(0, "Unspecified SerialPort Exception") {}
    SerialPortException(int code, const std::string& msg) throw() : IoException(code, msg.c_str()) {}
    SerialPortException(const SerialPortException& ve) throw() : IoException(ve) {}
    SerialPortException& operator=(const SerialPortException& other) throw() { IoException::operator=(other); return *this; }
}; // SerialPortException

/// \brief Invalid/unsupported baud rate
/// \ingroup io
class GRAPEIO_DLL_API InvalidBaudException : public Grape::SerialPortException
{
public:
    InvalidBaudException() throw() : SerialPortException(0, "Invalid/unsupported baud rate") {}
    InvalidBaudException(int code, const std::string& msg) throw() : SerialPortException(code, msg.c_str()) {}
    InvalidBaudException(const InvalidBaudException& ve) throw() : SerialPortException(ve) {}
    InvalidBaudException& operator=(const InvalidBaudException& other) throw() { SerialPortException::operator=(other); return *this; }
}; // SerialPortException

/// \brief Invalid/unsupported serial data format
/// \ingroup io
class GRAPEIO_DLL_API InvalidSerialDataFormatException : public Grape::SerialPortException
{
public:
    InvalidSerialDataFormatException() throw() : SerialPortException(0, "Invalid/unsupported baud rate") {}
    InvalidSerialDataFormatException(int code, const std::string& msg) throw() : SerialPortException(code, msg.c_str()) {}
    InvalidSerialDataFormatException(const InvalidBaudException& ve) throw() : SerialPortException(ve) {}
    InvalidSerialDataFormatException& operator=(const InvalidBaudException& other) throw() { SerialPortException::operator=(other); return *this; }
}; // SerialPortException

} // Grape

#endif // GRAPE_SERIALPORTEXCEPTION_H
