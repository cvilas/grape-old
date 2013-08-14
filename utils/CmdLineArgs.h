//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : CmdLineArgs.h
// Brief    : Command line arguments parser
//
// Copyright (c) 2012, Vilas Chitrakaran
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================

#ifndef GRAPE_CMDLINEARGS_H
#define GRAPE_CMDLINEARGS_H

#include "stringfuncs.h"
#include <map>

namespace Grape
{
    
    /// \class CmdLineArgs
    /// \ingroup utils
    /// \brief Command line parser.
    ///
    /// To use this class, command line options must be specified as:
    ///     \code
    ///     program -option1Name option1Value -option2Name option2Value ...
    ///     \endcode
    class GRAPEUTILS_DLL_API CmdLineArgs
    {
    public:
        /// Construct parser
        /// \param argc, argv Command line arguments
        CmdLineArgs(int argc, char** argv);
        
        ~CmdLineArgs() {}
        
        /// \return The number of options specified in the command line
        unsigned int getNumOptions() const { return _map.size(); }
        
        /// Get a command line option and value by index
        /// \param i       A numerical index in range [0, getNumOptions()]
        /// \param option  The option name at i-th index (output).
        /// \param value   The value, as string (output)
		/// \return  true on success, false if index is invalid.
        inline bool getOptionByIndex(unsigned int i, std::string &option, std::string& value);
        
        /// Get the value specified for a command line option
        /// \param option  The command line option (without the '-'). (input)
        /// \param dflt    The (default) value to return if the above option is
        ///                not found in command line. (input)
        /// \return  The value of the specified option.
        template <class T> 
        inline T getOption(const std::string& option, const T& dflt);
        
    private:
        std::map<std::string, std::string> _map;
        typedef std::map<std::string, std::string>::const_iterator  ConstEntryIter;
    };

    //------------------------------------------------------------------------------
    template<class T> T CmdLineArgs::getOption(const std::string& option, const T& dflt)
    //------------------------------------------------------------------------------
    {
        ConstEntryIter it = _map.find(option);
        if( it == _map.end() )
        {
            return dflt;
        }
        return Grape::stringAsT<T>(Grape::removeEndWhiteSpace(it->second));
    }
    
    //------------------------------------------------------------------------------
    bool CmdLineArgs::getOptionByIndex(unsigned int n, std::string &option, std::string& value)
    //------------------------------------------------------------------------------
    {
        if( n < getNumOptions() )
        {
            ConstEntryIter it = _map.begin();
            std::advance(it, n);
            option = it->first;
            value = Grape::removeEndWhiteSpace(it->second);
            return true;
        }
        return false;
    }

} // Grape

#endif // GRAPE_CMDLINEARGS_H

