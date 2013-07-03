//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoQtg.h
// Brief    : common header for IV classes
//==============================================================================

#ifndef GRAPE_SOQTG_H
#define GRAPE_SOQTG_H

#include "grapegraphics_common.h"
#include <Inventor/C/basic.h>

#if !defined(__COIN__)
#	error Coin library required for OpenInventor support
#endif

#if COIN_MAJOR_VERSION < 3
#	error Coin Version 3.0 and above is required
#endif

namespace Grape
{

/// \class SoQtg
/// \ingroup graphics
/// \brief The SoQtg class is used to initialize Open-Inventor related classes
/// (class names prefixed with 'So'..).
///
/// The sole function of this class is to be a placeholder for the toplevel
/// initialization code for all open-inventor related classes.
class GRAPEGRAPHICS_DLL_API SoQtg
{
public:

    /// Initialize Open-Inventor extention classes. This must be the first
    /// method called before using any other methods in the Open-Inventor extension
    /// classes. (Open-Inventor extension classes names have the 'So' prefix.)
    static void init();

    /// \return true if top level initialisation routine SoQtg::init() has been called.
    static bool isInit() { return _isInit; }

private:
    SoQtg();
    ~SoQtg();

private:
    static bool _isInit;

}; // SoQtg

} // Grape

#endif // GRAPE_SOQTG_H
