//==============================================================================
// Project  : Grape
// Module   : Core
// File     : StatusMessage.h
// Brief    : Status message
//==============================================================================

#ifndef GRAPE_STATUS_H
#define GRAPE_STATUS_H

#include "grapecore_common.h"
#include <sstream>
#include <string>

namespace Grape
{

/// \class Status
/// \ingroup core
/// \brief Holder for a single message and code
class GRAPECORE_DLL_API Status
{
public:
    Status() : _code(0) {}
    ~Status() {}

    /// Set code and message. Example usage:
    /// \code
    /// Status::set(code) << "Message goes here" << std::endl;
    /// \endcode
    std::ostringstream& set(long long int code);

    std::string getMessage() const { return _stream.str(); }

    long long int getCode() const { return _code; }

    void clear();

private:
    std::ostringstream  _stream;
    long long int       _code;
}; // Status

} // Grape

#endif // GRAPE_STATUS_H
