//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : ISignalerField.h
// Brief    : Qt field that emits a signal when value is changed
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

#ifndef GRAPE_ISIGNALERFIELD_H
#define GRAPE_ISIGNALERFIELD_H

#include "grapegraphics_common.h"
#include <QObject>

namespace grape
{

/// \class ISignalerField
/// \ingroup graphics
/// \brief Qt fields that raise a signal when modified
///
/// Users won't directly use this class. See ISignalerFieldT.
class GRAPEGRAPHICS_DLL_API ISignalerField : public QObject
{
    Q_OBJECT

public:
signals:
    void modified(); //!< Emit this signal when value of the field is modified

protected:
    ISignalerField() {}
    virtual ~ISignalerField() {}
}; // ISignalerField

/// \class ISignalerFieldT
/// \ingroup graphics
/// \brief Qt fields that raise a signal when modified
///
/// This class encapsulates a field of user specified type and raises a
/// signal when the value of that field is changed.
template <class T>
class GRAPEGRAPHICS_DLL_API ISignalerFieldT : public ISignalerField
{
public:
    inline void operator=(const T& val); //!< set a value and emit signal modified()
    inline const T& value() const;
private:
    T _value;
}; // ISignalerFieldT


//==============================================================================
template <class T>
void ISignalerFieldT<T>::operator =(const T& val)
//==============================================================================
{
    _value = val;
    emit ISignalerField::modified();
}

//------------------------------------------------------------------------------
template <class T>
const T& ISignalerFieldT<T>::value() const
//------------------------------------------------------------------------------
{
    return _value;
}

} // grape

#endif // GRAPE_ISIGNALERFIELD_H
