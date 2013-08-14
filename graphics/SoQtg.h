//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoQtg.h
// Brief    : common header for IV classes
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

#ifndef GRAPE_SOQTG_H
#define GRAPE_SOQTG_H

#include "grapegraphics_common.h"
#include <Inventor/C/basic.h>

#if defined(__ANDROID__)
#   error Coin support is not available on Android
#endif

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
    static bool isInit() { return getSingleton()._isInit; }

private:
    /// \return Reference to single me
    inline static SoQtg& getSingleton();

    SoQtg() {}
    ~SoQtg() {}
    SoQtg(const SoQtg&);
    SoQtg& operator=(const SoQtg&);

private:
    bool _isInit;

}; // SoQtg

//------------------------------------------------------------------------------
SoQtg& SoQtg::getSingleton()
//------------------------------------------------------------------------------
{
    static SoQtg s_singleton;
    return s_singleton;
}

} // Grape

#endif // GRAPE_SOQTG_H
