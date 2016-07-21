//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : TimeAnnotation.h
// Brief    : Time annotations on QImages
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

#ifndef GRAPE_TIMEANNOTATION_H
#define GRAPE_TIMEANNOTATION_H

#include "TextAnnotation.h"

namespace grape
{

/// \class TimeAnnotation
/// \ingroup graphics
/// \brief Time-stamp annotation. Used with ImageAnnotator.
///
/// This is a specialised class used for placing time-stamps on a QImage. An
/// object of this class is used with ImageAnnotator.
///
/// This is a class derived from TextAnnotation. On every call
/// to ImageAnnotator::annotate(), an object of this class updates
/// TextAnnotation::text field with the current time stamp. (Therefore, setting
/// this field manually has not effect). The time-stamp format is set by the
/// TimeAnnotation::format field which follows the convention for the
/// format used by QDateTime::toString(QString). See documentation for QDateTime.
///
/// Example program:
/// See example for ImageAnnotator class
class GRAPEGRAPHICS_DLL_API TimeAnnotation : public TextAnnotation
{
    Q_OBJECT
public:
    QString format; //!< format string. See documentation for QDateTime::toString(QString). The default is "dd MMM yyyy - hh:mm:ss.zzz".
public:
    TimeAnnotation(QObject *parent = 0);
    virtual ~TimeAnnotation();
protected:
    void process(QPainter& painter);
}; // TimeAnnotation

} // grape

#endif // GRAPE_TIMEANNOTATION_H
