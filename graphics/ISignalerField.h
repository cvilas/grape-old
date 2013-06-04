//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : ISignalerField.h
// Brief    : Qt field that emits a signal when value is changed
//==============================================================================

#ifndef GRAPE_ISIGNALERFIELD_H
#define GRAPE_ISIGNALERFIELD_H

#include "grapegraphics_common.h"
#include <QObject>

namespace Grape
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

} // Grape

#endif // GRAPE_ISIGNALERFIELD_H
