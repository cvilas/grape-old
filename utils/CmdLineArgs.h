//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : CmdLineArgs.h
// Brief    : Command line arguments parser
//==============================================================================

#ifndef GRAPE_CMDLINEARGS_H
#define GRAPE_CMDLINEARGS_H

#include "stringfuncs.h"
#include <map>

namespace Grape
{
    
    /// \class CmdLineArgs
    /// \ingroup utils
    /// \brief Retrieves command line options.
    ///
    /// To use this class, command line options must be specified as:
    /// \code
    /// program -option1Name option1Value -option2Name option2Value ...
    /// \endcode
    ///
    /// <b>Example Program:</b>
    /// \include CmdLineArgsExample.cpp

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

