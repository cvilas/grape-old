//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : Config.h
// Brief    : Configuration strings
//==============================================================================

#ifndef GRAPE_CONFIG_H
#define GRAPE_CONFIG_H

#include "stringfuncs.h"
#include <map>
#include <iostream>

namespace Grape
{

/// \class Config
/// \ingroup utils
/// \brief String list of key/value/comment
///
/// A config class holds a list of key/value/comment triplets, where
/// the key, the value and the comment are all strings.
///
/// Example program:
/// see ConfigFileExample.cpp
class GRAPEUTILS_DLL_API Config
{

public:

    // ------------------ construction --------------------

    Config() {}
    Config(const Config& cfg) { _entries = cfg._entries; }
    ~Config() {}
    inline Config &operator=(const Config &sec);

    /// print to an output stream
    /// \param str output stream (example std::cout)
    /// \param lsp number of leading white spaces to print
    void print(std::ostream& str, unsigned int lsp = 0) const;

    /// read in key/value/comments from a stream. The data should be
    /// formatted as follows
    /// \code
    /// key = value; /'comment'/
    /// \endcode
    /// The comment blocks are optional.
    /// \param str input stream
    /// \param errorStream stream to output parse error messages to (default is standard error stream).
    /// \return true if data read successfully.
    bool parse(const std::string& str, std::ostream& errorStream = std::cerr);

    // ------------------ key/value entries --------------------

    /// Clear all contents
    void clear() { _entries.clear(); }

    /// Set value for an existing key or add a new key/value/comment triplet.
    /// \note The object of type T must have the following ostream
    /// redirection operator defined in order for this to work:
    /// \code
    /// std::ostream& operator<<(std::ostream &, const T &);
    /// \endcode
    /// \param key    An identifier name for the entry.
    /// \param value  The value assigned to the name.
    /// \param comment An optional comment string
    template<class T>
    void setEntry(const std::string &key, const T &value, const std::string& comment="");

    /// Read an entry by key.
    /// \note The object of type T must have the following istream
    /// redirection operator defined in order for this to work:
    /// \code
    /// std::istream& operator>>(std::istream &, const T &);
    /// \endcode
    /// \param key     The identifier name for the entry.
    /// \param value   The value associated with the key.
    /// \param comment  The comment associated with the entry
    /// \return true if entry key was found. False is returned if entry key is not found, in which
    /// case the contents of value and comment parameters are undefined.
    template<class T>
    bool getEntry(const std::string &key, T &value, std::string& comment) const;

    /// \return The number of entries held currently
    unsigned int getNumEntries() const { return _entries.size(); }

    /// Get the name of an entry by indexing into internal map.
    /// \note This index has nothing to do with the sequence in which the entry was stored
    /// in the object. It is an index into the internal map pre-sorted to improve efficiency
    /// in looking up a value by entry name.
    /// \param n The numerical index (starts at 0).
    /// \return  The name of the entry, or empty string if index is invalid.
    std::string getEntryKey(unsigned int n) const;

    /// Remove a named entry.
    /// \param key     Name of the entry to be removed.
    /// \return  true on success, false if entry not found.
    inline bool removeEntry(const std::string &key);

    /// Remove an entry by index.
    /// \note This index has nothing to do with the sequence in which the entry was stored
    /// in the object. It is an index into the internal map pre-sorted to improve
    /// efficiency in looking up a value by entry name.
    /// \param n The numerical index (starts at 0).
    /// \return  true on success, false if entry not found.
    bool removeEntry(unsigned int n);

private:

    /// \brief value/comment pair for a key
    class Entry
    {
    public:
        std::string value;
        std::string comment;
    };

    std::map<std::string/*key*/, Entry> _entries; //!< list of entries accessible by key

    typedef std::map<std::string/*key*/, Entry>::iterator       EntryIter;
    typedef std::map<std::string/*key*/, Entry>::const_iterator ConstEntryIter;
}; // Config


//==============================================================================
Config &Config::operator=(const Config &sec)
//==============================================================================
{
    if( this != &sec )
    {
        _entries = sec._entries;
    }
    return *this;
}

//------------------------------------------------------------------------------
template<class T>
void Config::setEntry(const std::string &key, const T &value, const std::string& comment)
//------------------------------------------------------------------------------
{
    Entry entry;
    entry.value = Grape::removeEndWhiteSpace( Grape::TAsString(value) );
    entry.comment = comment;
    _entries[ Grape::removeEndWhiteSpace(key) ] = entry;
}


//------------------------------------------------------------------------------
template<class T>
bool Config::getEntry(const std::string &key, T &value, std::string& comment) const
//------------------------------------------------------------------------------
{
    ConstEntryIter it = _entries.find( Grape::removeEndWhiteSpace(key) );
    if( it == _entries.end() )
    {
        return false;
    }
    value = Grape::stringAsT<T>(it->second.value);
    comment = it->second.comment;
    return true;
}

//------------------------------------------------------------------------------
bool Config::removeEntry(const std::string &key)
//------------------------------------------------------------------------------
{
    return ( 0 != _entries.erase( Grape::removeEndWhiteSpace(key) ) );
}


} // Grape

#endif // GRAPE_CONFIG_H
